package com.zipc.cockpit.server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Timer;

import org.kie.api.KieBaseConfiguration;
import org.kie.api.KieServices;
import org.kie.api.conf.EventProcessingOption;
import org.kie.api.runtime.KieContainer;
import org.kie.api.runtime.KieSession;
import org.kie.api.runtime.rule.EntryPoint;
import org.kie.internal.KnowledgeBaseFactory;

import zipc.brms.EventLevelData;
import zipc.brms.EventOutput;
import zipc.brms.EventCharacteristicAndStatus3sigma;
import zipc.brms.EventCharacteristicAndStatusBinomial;
import zipc.brms.EventSpeedAcceleration;
import zipc.brms.EventSpeedDeceleration;
import zipc.brms.EventDurationOfShortTTCCheck;
import zipc.brms.EventTTCOfSpeedDecelerationCheck;
import zipc.brms.EventStoppedDistanceCheck;
import zipc.brms.EventDistance;
import zipc.brms.EventRoadClass;
import zipc.brms.EventAccelOpen;
import zipc.brms.EventSpeed;

@SuppressWarnings("restriction")
public class DNCockpitManager
{
	static class theLock extends Object {}
	static public theLock lockObjectFire = new theLock();
	static public theLock lockObjectOutput = new theLock();
	static public Object GUIlockObject;
	
	// Droolsセッション関連定義
	public KieServices ks;
    public KieContainer kc;
    public KieSession sessionB;
    public KieBaseConfiguration config;

	private Timer timer1;
	private Timer timer2;
	private Timer timerOut;
    private DroolsFireTimer timeTask;
    private DroolsFireTimer timeTask2;
    private OutputStateTimer timerTaskOut;
    
    // ソケット受信部関連定義
	private IntegrationGenerateClientThread IGClientThread;
    private DecisionAppServerThread decisionAppServerThread;

    // 処理開始タイミング
    long start = System.currentTimeMillis();
    // ログ保存フラグ
    private boolean timeLinerFlg = false;
    
    // デバッグ用データ投入スレッド
    public DebugThead runThD;
    public DebugThead runThA;
    public DebugThead runThS;
    public DebugThead runThR;
    
    // GUI参照
    DNCockpitGUI gui;
	
	private GlobalManager globalData = null;
    
	public DNCockpitManager(DNCockpitGUI _gui)
	{
		gui = _gui;

	    // 設定ファイル読み込み
	    readSettingFile();

	    // Drools セッション生成
		ks = KieServices.Factory.get();
	    kc = ks.getKieClasspathContainer();
	    sessionB = kc.newKieSession("BRMSSession");
	    config = KnowledgeBaseFactory.newKnowledgeBaseConfiguration();
	    
	    // ユーザ特性ファイル読み込み
	    readUserSpecificDataFile();

		// グローバル変数を最初に定義する
	    globalData = new GlobalManager();
		
		try {
			// ストリームモード実行（タイムライン順に処理）
		    config.setOption( EventProcessingOption.STREAM );
			
		} catch (Exception e) {
			e.printStackTrace();
		}

		// ルール起動初期化
		sessionB.setGlobal("list", getGlobalManager().getList());
		sessionB.getAgenda().getAgendaGroup("StartUpAgenda").setFocus();
		sessionB.fireAllRules();
		sessionB.getAgenda().getAgendaGroup("StartUpAgenda").clear();
		
	    // File Timer 起動
		timer1 = new Timer();
		timeTask = new DroolsFireTimer(this, "SpecificAgenda", sessionB);
        timer1.schedule(timeTask, 0, 500); 
		timer2 = new Timer();
		timeTask2 = new DroolsFireTimer(this, "SpecificAgenda10s", sessionB);
        timer2.schedule(timeTask2, 10000, 10000); 

        globalData.getOutput().addConsoleLog("Droolsセッション起動完了\n");
        this.updateGUI(globalData.getOutput());
        
        if(timeLinerFlg)
        {
        	timerOut = new Timer();
        	timerTaskOut = new OutputStateTimer(this);
        	timerOut.schedule(timerTaskOut, 1000, 1000);
        }
        
        // CAN受信スレッド起動
        IGClientThread = new IntegrationGenerateClientThread(this, sessionB);
        IGClientThread.start();
        decisionAppServerThread = new DecisionAppServerThread(this, sessionB);
        decisionAppServerThread.start();
	}
	// 設定ファイル読みこみ
	private void readSettingFile()
	{
		// 設定ファイル読み込み
		File file = new File("C:\\work\\brms\\setting.txt");
		try {
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file),"MS932"));
			
			while(true)
			{
				String tmp =  br.readLine();
				if(tmp == null)
				{
					// ファイル終端につき終了
					break;
				}
				String[] div = tmp.split(":");
				switch(div[0])
				{
				case "TimerLiner連携":
					if(div[1].equals("ON"))
					{
						timeLinerFlg = true;
					}
					break;
				case "画面表示":
					if(div[1].equals("ON")) {
						gui.setSize(1329, 658);
			        	gui.setLocationRelativeTo(null);
						gui.setVisible(true);
					}
					break;
				default:
					// 無視
					break;
				}
			}
			br.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
    
	// ユーザ特性ファイル読み込み
	private void readUserSpecificDataFile()
	{
		// 設定ファイル読み込み
		File file = new File("C:\\work\\brms\\UserSpecificData.txt");
		
		EventSpeedAcceleration accelSpecific = new EventSpeedAcceleration();
		EventSpeedDeceleration reduceSpecific = new EventSpeedDeceleration();
		EventTTCOfSpeedDecelerationCheck reduceStart = new EventTTCOfSpeedDecelerationCheck();
		EventDurationOfShortTTCCheck TTCRisk = new EventDurationOfShortTTCCheck();
		EventCharacteristicAndStatusBinomial distanceOnOpenroad = new EventCharacteristicAndStatusBinomial();
		EventCharacteristicAndStatusBinomial distanceOnExpressway = new EventCharacteristicAndStatusBinomial();
		EventStoppedDistanceCheck distanceOnFollowStop = new EventStoppedDistanceCheck();

		try {
			if(file.exists())
			{
				BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file),"MS932"));
				
				System.out.println("ユーザ特性ファイル読み込み開始");
				while(true)
				{
					String tmp =  br.readLine();
					if(tmp == null)
					{
						// ファイル終端につき終了
						break;
					}
					String[] div = tmp.split(":");
					switch(div[0])
					{
					case "急加速頻度":
						accelSpecific.setOldSamplePercentage(Double.valueOf(div[1]));
						if(div[2].equals("NORMAL"))
						{
							accelSpecific.setOldSampleSpecific("NORMAL");
						}
						else if(div[2].equals("SUDDEN"))
						{
							accelSpecific.setOldSampleSpecific("SUDDEN");
						}
						else
						{
							accelSpecific.setOldSampleSpecific("NAN");
						}
						
						for(int i=3; i<div.length;i++ )
						{
							if(div[i].equals("SUDDEN"))
							{
								accelSpecific.addSample("SUDDEN");
							}
							else if(div[i].equals("NORMAL"))
							{
								accelSpecific.addSample("NORMAL");
							}
						}
						if(div.length >= 103 )
						{
							accelSpecific.setUserStateCheck(true);
						}
						break;
					case "急減速頻度":
						reduceSpecific.setOldSamplePercentage(Double.valueOf(div[1]));
						if(div[2].equals("NORMAL"))
						{
							reduceSpecific.setOldSampleSpecific("NORMAL");
						}
						else if(div[2].equals("SUDDEN"))
						{
							reduceSpecific.setOldSampleSpecific("SUDDEN");
						}
						else
						{
							reduceSpecific.setOldSampleSpecific("NAN");
						}
						
						for(int i=3; i<div.length;i++ )
						{
							if(div[i].equals("SUDDEN"))
							{
								reduceSpecific.addSample("SUDDEN");
							}
							else if(div[i].equals("NORMAL"))
							{
								reduceSpecific.addSample("NORMAL");
							}
						}
						if(div.length >= 103 )
						{
							reduceSpecific.setUserStateCheck(true);
						}
						break;
					case "他車追従時の減速開始タイミング":
						reduceStart.setOldSampleAverage(Double.valueOf(div[1]));
						reduceStart.setOldSampleStdDeviation(Double.valueOf(div[2]));
						
						for(int i=3; i<div.length;i++ )
						{
							reduceStart.addSample(Double.valueOf(div[i]));
						}
						if(div.length >= 53)
						{
							reduceStart.setUserStateCheck(true);
						}
						break;
					case "TTC短時間へのリスク感覚":
						TTCRisk.setOldSampleAverage(Double.valueOf(div[1]));
						TTCRisk.setOldSampleStdDeviation(Double.valueOf(div[2]));
						
						for(int i=3; i<div.length;i++ )
						{
							TTCRisk.addSample(Double.valueOf(div[i]));
						}
						if(div.length >= 53)
						{
							TTCRisk.setUserStateCheck(true);
						}
						
						break;
					case "高速道路走行時の平均車間距離":
						distanceOnExpressway.setOldSamplePercentage(Double.valueOf(div[1]));
						if(div[2].equals("MARGIN_LARGE"))
						{
							distanceOnExpressway.setOldSampleSpecific("MARGIN_LARGE");
						}
						else if(div[2].equals("MARGIN_MEDIUM"))
						{
							distanceOnExpressway.setOldSampleSpecific("MARGIN_MEDIUM");
						}
						else if(div[2].equals("MARGIN_SMALL"))
						{
							distanceOnExpressway.setOldSampleSpecific("MARGIN_SMALL");
						}
						else
						{
							distanceOnExpressway.setOldSampleSpecific("NAN");
						}
						
						for(int i=3; i<div.length;i++ )
						{
							if(div[i].equals("MARGIN_LARGE"))
							{
								distanceOnExpressway.addSample("MARGIN_LARGE");
							}
							else if(div[i].equals("MARGIN_MEDIUM"))
							{
								distanceOnExpressway.addSample("MARGIN_MEDIUM");
							}
							else if(div[i].equals("MARGIN_SMALL"))
							{
								distanceOnExpressway.addSample("MARGIN_SMALL");
							}
						}
						if(div.length >= 103 )
						{
							distanceOnExpressway.setUserStateCheck(true);
						}
						break;
					case "一般道路走行時の平均車間距離":
						distanceOnOpenroad.setOldSamplePercentage(Double.valueOf(div[1]));
						if(div[2].equals("MARGIN_LARGE"))
						{
							distanceOnOpenroad.setOldSampleSpecific("MARGIN_LARGE");
						}
						else if(div[2].equals("MARGIN_MEDIUM"))
						{
							distanceOnOpenroad.setOldSampleSpecific("MARGIN_MEDIUM");
						}
						else if(div[2].equals("MARGIN_SMALL"))
						{
							distanceOnOpenroad.setOldSampleSpecific("MARGIN_SMALL");
						}
						else
						{
							distanceOnOpenroad.setOldSampleSpecific("NAN");
						}
						
						for(int i=3; i<div.length;i++ )
						{
							if(div[i].equals("MARGIN_LARGE"))
							{
								distanceOnOpenroad.addSample("MARGIN_LARGE");
							}
							else if(div[i].equals("MARGIN_MEDIUM"))
							{
								distanceOnOpenroad.addSample("MARGIN_MEDIUM");
							}
							else if(div[i].equals("MARGIN_SMALL"))
							{
								distanceOnOpenroad.addSample("MARGIN_SMALL");
							}
						}
						if(div.length >= 103 )
						{
							distanceOnOpenroad.setUserStateCheck(true);
						}
						break;
					case "追従停車時の車間距離":
						distanceOnFollowStop.setOldSampleAverage(Double.valueOf(div[1]));
						distanceOnFollowStop.setOldSampleStdDeviation(Double.valueOf(div[2]));

						for(int i=3; i<div.length;i++ )
						{
							distanceOnFollowStop.addSample(Double.valueOf(div[i]));
						}
						if(div.length >= 103 )
						{
							distanceOnFollowStop.setUserStateCheck(true);
						}
						break;
					default:
						// 無視
						break;
					}
				}
				br.close();
				br = null;
			}

			EntryPoint profileStream = sessionB.getEntryPoint( "BRMS Work Stream" );
			accelSpecific.setType("PROFILE_TYPE_BINOMIAL.SPEED_ACCELERATION");
			profileStream.insert(accelSpecific);
			reduceSpecific.setType("PROFILE_TYPE_BINOMIAL.SPEED_DECELERATION");
			profileStream.insert(reduceSpecific);
			reduceStart.setType("PROFILE_TYPE_THREESIGMA.TTC_OF_SPEED_DECELERATION");
			profileStream.insert(reduceStart);
			TTCRisk.setType("PROFILE_TYPE_THREESIGMA.DURATION_OF_SHORTTTC_CHECK");
			profileStream.insert(TTCRisk);
			distanceOnOpenroad.setType("PROFILE_TYPE_BINOMIAL.RUNNING_DISTANCE_ONOPENROAD");
			profileStream.insert(distanceOnOpenroad);
			distanceOnExpressway.setType("PROFILE_TYPE_BINOMIAL.RUNNING_DISTANCE_ONEXPRESSWAY");
			profileStream.insert(distanceOnExpressway);
			distanceOnFollowStop.setType("PROFILE_TYPE_THREESIGMA.STOPPED_DISTANCE");
			distanceOnFollowStop.setMAX_SAMPLE_SIZE(100);
			profileStream.insert(distanceOnFollowStop);
			
			accelSpecific = null;
			reduceStart = null;
			TTCRisk = null;
			distanceOnOpenroad = null;
			distanceOnExpressway = null;
			distanceOnFollowStop = null;
			profileStream = null;
			file = null;
			System.out.println("");
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	// ユーザ特性ファイル書き込み
	public void writeUserSpecificDataFile()
	{
		EntryPoint profileStream = sessionB.getEntryPoint( "BRMS Work Stream" );
		
		Object[] obj = profileStream.getObjects().toArray();

		EventSpeedAcceleration accelSpecific = null;
		EventSpeedDeceleration reduceSpecific = null;
		EventTTCOfSpeedDecelerationCheck reduceStart = null;
		EventDurationOfShortTTCCheck TTCRisk = null;
		EventCharacteristicAndStatusBinomial distanceOnOpenroad = null;
		EventCharacteristicAndStatusBinomial distanceOnExpressway = null;
		EventStoppedDistanceCheck distanceOnFollowStop = null;
		
		for(int i=0; i<obj.length;i++)
		{
			
			if(obj[i].getClass().equals(EventSpeedAcceleration.class))
			{
				accelSpecific = (EventSpeedAcceleration)obj[i];
			}
			else if(obj[i].getClass().equals(EventSpeedDeceleration.class))
			{
				reduceSpecific = (EventSpeedDeceleration)obj[i];
			}
			else if(obj[i].getClass().equals(EventCharacteristicAndStatusBinomial.class))
			{
				if(((EventCharacteristicAndStatusBinomial)obj[i]).getType() == "PROFILE_TYPE_BINOMIAL.RUNNING_DISTANCE_ONOPENROAD")
				{
					distanceOnOpenroad = (EventCharacteristicAndStatusBinomial)obj[i];
				}
				else if(((EventCharacteristicAndStatusBinomial)obj[i]).getType() == "PROFILE_TYPE_BINOMIAL.RUNNING_DISTANCE_ONEXPRESSWAY")
				{
					distanceOnExpressway = (EventCharacteristicAndStatusBinomial)obj[i];
				}
			}
			else if(obj[i].getClass().equals(EventTTCOfSpeedDecelerationCheck.class))
			{
				reduceStart = (EventTTCOfSpeedDecelerationCheck)obj[i];
			}
			else if(obj[i].getClass().equals(EventDurationOfShortTTCCheck.class))
			{
				TTCRisk = (EventDurationOfShortTTCCheck)obj[i];
			}
			else if(obj[i].getClass().equals(EventStoppedDistanceCheck.class))
			{
				distanceOnFollowStop = (EventStoppedDistanceCheck)obj[i];
			}
		}
		
		Queue<String> copyQueue = accelSpecific.getSampling();
    	Queue<String> tmpAccelSpecific = new LinkedList<String>();
    	int x = 0;
    	if(copyQueue.size()>accelSpecific.MAX_SAMPLE_SIZE)
    	{
    		x = copyQueue.size() - accelSpecific.MAX_SAMPLE_SIZE;
    	}
    	for(; x < copyQueue.size(); x++)
    	{
    		tmpAccelSpecific.add((String)copyQueue.toArray()[x]);
    	}
    	
    	x = 0;
		copyQueue = reduceSpecific.getSampling();
    	Queue<String> tmpReduceSpecific = new LinkedList<String>();
    	if(copyQueue.size()>reduceSpecific.MAX_SAMPLE_SIZE)
    	{
    		x = copyQueue.size() - reduceSpecific.MAX_SAMPLE_SIZE;
    	}
    	for(; x < copyQueue.size(); x++)
    	{
    		tmpReduceSpecific.add((String)copyQueue.toArray()[x]);
    	}
		
    	x = 0;
    	copyQueue = distanceOnOpenroad.getSampling();
    	Queue<String> tmpOpenroad = new LinkedList<String>();
    	if(copyQueue.size()>distanceOnOpenroad.MAX_SAMPLE_SIZE)
    	{
    		x = copyQueue.size() - distanceOnOpenroad.MAX_SAMPLE_SIZE;
    	}
    	for(; x < copyQueue.size(); x++)
    	{
    		tmpOpenroad.add((String)copyQueue.toArray()[x]);
    	}
		
    	x = 0;
    	copyQueue = distanceOnExpressway.getSampling();
    	Queue<String> tmpExpressway = new LinkedList<String>();
    	if(copyQueue.size()>distanceOnExpressway.MAX_SAMPLE_SIZE)
    	{
    		x = copyQueue.size() - distanceOnExpressway.MAX_SAMPLE_SIZE;
    	}
    	for(; x < copyQueue.size(); x++)
    	{
    		tmpExpressway.add((String)copyQueue.toArray()[x]);
    	}	
		
		Queue<Double> tmpReduceStart = reduceStart.getAllSample();
		Queue<Double> tmpTTC = TTCRisk.getAllSample();
		Queue<Double> tmpFollowStop = distanceOnFollowStop.getAllSample();
		
		// 設定ファイル書き込み
		try {
			File file = new File("C:\\work\\brms\\UserSpecificData.txt");
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file),"MS932"));

			bw.write("急加速頻度:" +accelSpecific.getPercentage()+":" + accelSpecific.getSpecificENUM());
			for(int i=0; i<tmpAccelSpecific.size();i++)
			{
				bw.write(":" + tmpAccelSpecific.toArray()[i]);
			}
			bw.write("\n");
			
			bw.write("急減速頻度:" +reduceSpecific.getPercentage()+":" + reduceSpecific.getSpecificENUM());
			for(int i=0; i<tmpReduceSpecific.size();i++)
			{
				bw.write(":" + tmpReduceSpecific.toArray()[i]);
			}
			bw.write("\n");
			
			bw.write("他車追従時の減速開始タイミング:" + reduceStart.getAverage() + ":" + reduceStart.getStdDeviation());
			for(int i=0; i<tmpReduceStart.size();i++)
			{
				bw.write(":" + tmpReduceStart.toArray()[i]);
			}
			bw.write("\n");
			
			bw.write("TTC短時間へのリスク感覚:" + TTCRisk.getAverage() + ":" + TTCRisk.getStdDeviation());
			for(int i=0; i<tmpTTC.size();i++)
			{
				bw.write(":" + tmpTTC.toArray()[i]);
			}
			bw.write("\n");

			bw.write("一般道路走行時の平均車間距離:" + distanceOnOpenroad.getPercentage() + ":" + distanceOnOpenroad.getSpecificENUM());
			for(int i=0; i<tmpOpenroad.size();i++)
			{
				bw.write(":" + tmpOpenroad.toArray()[i]);
			}
			bw.write("\n");

			bw.write("高速道路走行時の平均車間距離:" + distanceOnExpressway.getPercentage() + ":" + distanceOnExpressway.getSpecificENUM());
			for(int i=0; i<tmpExpressway.size();i++)
			{
				bw.write(":" + tmpExpressway.toArray()[i]);
			}
			bw.write("\n");
			
			bw.write("追従停車時の車間距離:" + distanceOnFollowStop.getAverage() + ":" + distanceOnFollowStop.getStdDeviation());
			for(int i=0; i<tmpFollowStop.size();i++)
			{
				bw.write(":" + tmpFollowStop.toArray()[i]);
			}
			bw.write("\n");
			
			//ファイルを閉じる
            bw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		finally{
			tmpAccelSpecific = null;
			tmpReduceSpecific = null;
			tmpReduceStart = null;
			tmpTTC = null;
			tmpOpenroad = null;
			tmpExpressway = null;
			tmpFollowStop = null;
		}
	}

	// 処理終了
    public void halt()
    {
		System.out.println("Timer1 stop");
		timeTask.halt();
		timer1.cancel();
		timeTask2.halt();
		timer2.cancel();
		if(timeLinerFlg)
		{
			timerTaskOut.halt();
			timerOut.cancel();
		}
		System.out.println("Socket stop");
		IGClientThread.halt();
		decisionAppServerThread.halt();
		System.out.println("Debug stop");
		stopDebugMode();
        /*
         * 標準出力をデフォルトに戻す
         */
    }
	
	// ロック参照取得 Fire用
	public Object getLockObjectFire()
	{
		return lockObjectFire;
	}

	// ロック参照取得 Output用
	public Object getLockObjectOutput()
	{
		return lockObjectOutput;
	}

	// デバッグモード実行
	public void runDebugMode()
	{
		System.out.println("デバッグモード起動");
		runThD = new DebugThead(SIGTYPE.DISTANCE, 48);
		runThD.start(); 
		runThA = new DebugThead(SIGTYPE.ACCELOPEN, 48);
		runThA.start(); 
        runThS = new DebugThead(SIGTYPE.SPEED, 24);
        runThS.start();
        runThR = new DebugThead(SIGTYPE.ROADCLASS, 1000);
        runThR.start();
		
	}
	// デバッグモード終了
	public void stopDebugMode()
	{
		System.out.println("デバッグモード終了");
		try
		{
			runThD.halt();
			runThA.halt();
			runThS.halt();
			runThR.halt();
		}
		catch(Exception e)
		{
			
		}
		finally
		{
			runThD = null;
			runThS = null;
			runThA = null;
			runThR = null;
		}
	}
	
	public class GlobalManager
	{
		private EventOutput list;
		private EventOutput output;
		
	    public GlobalManager()
	    {
		    list = new EventOutput("停車","前車なし","","","未定義","状態未判定","未定義","状態未判定","未定義","状態未判定","未定義","状態未判定","未定義","状態未判定","未定義","状態未判定","未定義","状態未判定",0,0,0,0,0,0,0);
	    	output = new EventOutput();
	    }

	    public EventOutput getList() {
	    	return this.list;
	    }
	    public EventOutput getOutput() {
	    	return this.output;
	    }
	}

	public void updateGUI(EventOutput output)
	{
		gui.updateGUI(output);
	}

	public GlobalManager getGlobalManager()
	{
		return globalData;
	}
	
	enum SIGTYPE {DISTANCE, SPEED, ACCELOPEN, ROADCLASS};
	
	// デバッグ用データ投入スレッド
	class DebugThead extends Thread{
		
		private SIGTYPE sig;
		private boolean stopflg = false;
		private long sleepTime;
		DebugThead(SIGTYPE _sig, long _sleepTime)
		{
			sig = _sig;
			sleepTime = _sleepTime;
		}
		
		public void run()
		{ 
			while(!stopflg)
			{
				try
				{
					try {
						Thread.sleep(sleepTime);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					
					// TimeLiner連携初期時間設定
					if(gui.starttime == 0)
					{
						gui.starttime = System.currentTimeMillis();
					}
					
		        	synchronized(getLockObjectFire()){
						EntryPoint cockpitStream = sessionB.getEntryPoint( "BRMS Work Stream" );
						if(sig == SIGTYPE.SPEED)
						{
					        EventSpeed tmpSpeed = new EventSpeed(Long.valueOf(gui.getSpeed())*100, "TYPE_SPEED.VEHICLE_SPEED_SP1");
					       	cockpitStream.insert(tmpSpeed);
					        tmpSpeed = null;
					        
						}
						else if(sig == SIGTYPE.DISTANCE)
						{
							EventDistance tmpDistance = new EventDistance(Long.valueOf(gui.getDistance())*100, "TYPE_DISTANCE.VEHICLE_FOLLOWING_DISTANCE");
					        cockpitStream.insert(tmpDistance);
					        
							// 負荷レベル
							EventLevelData tmplevel = new EventLevelData(3, "TYPE_LEVEL.LOAD_LEVEL");
							EventLevelData tmplevelf = new EventLevelData(4,"TYPE_LEVEL.AIMLESS_LEVEL");
					        cockpitStream.insert(tmplevel);
					        cockpitStream.insert(tmplevelf);
					        tmpDistance = null;
						}
						else if(sig == SIGTYPE.ACCELOPEN)
						{
							EventAccelOpen tmpAccelOpen = new EventAccelOpen(Long.valueOf(gui.getAccelOpen())*10, "TYPE_ACCELOPEN.ACCEL_OPEN");
					        cockpitStream.insert(tmpAccelOpen);
					        tmpAccelOpen = null;
						}
						else if(sig == SIGTYPE.ROADCLASS)
						{
							EventRoadClass tmpRoadClass = new EventRoadClass(gui.getRoadClass(), "TYPE_ROAD_CLASS.ROAD_CLASS");
					        cockpitStream.insert(tmpRoadClass);
					        tmpRoadClass = null;
						}
						cockpitStream = null;
		        	}
				}
				catch(Exception e)
				{
					System.out.println("catch");
        			e.printStackTrace();
				}
			}
		}
		public void halt()
		{ 
			stopflg = true;
		}
	}
}
