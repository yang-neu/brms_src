package com.zipc.cockpit.server;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.TimerTask;

import zipc.brms.EventOutput;

public class OutputStateTimer extends TimerTask {
	private boolean TimelinerConfigENABLE = true;

	private File file;

	private DNCockpitManager parent;
	private BufferedWriter bw;
	private TimelinerConfig tc;

	// コンストラクタ
	public OutputStateTimer(DNCockpitManager parent) {
		this.parent = parent;
		String OutputFileName = "C:\\work\\brms\\TimeLinerData.txt"; //$NON-NLS-1$
		file = new File(OutputFileName);
		try {

			bw = new BufferedWriter(
					new OutputStreamWriter(new FileOutputStream(file), Messages.getString("OutputStateTimer.1"))); //$NON-NLS-1$
			if (TimelinerConfigENABLE) {
				tc = TimelinerConfig.ParseConfig("C:\\work\\brms\\Timeliner.conf");
				
				bw.write("time,");
				tc.printHeader(bw);
			}
		} catch (IOException e) {
			// TODO 自動生成された catch ブロック
			e.printStackTrace();
		}
	}

	private String getTimeStr(long time) {
		long sec = ((time / 1000));

		return (String.format("%03d", sec)); //$NON-NLS-1$
	}

	@Override
	public void run() {

		EventOutput list = parent.getGlobalManager().getOutput();

		if (parent.gui.starttime == 0) {
			return;
		}

		long time = System.currentTimeMillis();
		
		if (TimelinerConfigENABLE) {
			try {
				bw.write(getTimeStr(time - parent.gui.starttime) + ","); //$NON-NLS-1$
				tc.printData(list, bw);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			try {
				// 運転シーン
				String driveS = ""; //$NON-NLS-1$
				switch (list.getDriveScene()) {
				case "停車": //$NON-NLS-1$
					driveS = Messages.getString("OutputStateTimer.5"); //$NON-NLS-1$
					break;
				case "発進": //$NON-NLS-1$
					driveS = Messages.getString("OutputStateTimer.7"); //$NON-NLS-1$
					break;
				case "定常走行": //$NON-NLS-1$
					driveS = Messages.getString("OutputStateTimer.9"); //$NON-NLS-1$
					break;
				case "加速走行": //$NON-NLS-1$
					driveS = Messages.getString("OutputStateTimer.11"); //$NON-NLS-1$
					break;
				case "減速走行": //$NON-NLS-1$
					driveS = Messages.getString("OutputStateTimer.13"); //$NON-NLS-1$
					break;
	
				default:
					break;
	
				}
				bw.write(getTimeStr(time - parent.gui.starttime) + "," + driveS); //$NON-NLS-1$
	
				// 前車有無シーン
				String frontS = ""; //$NON-NLS-1$
				switch (list.getWithFrontCar()) {
				case "前車なし": //$NON-NLS-1$
					frontS = Messages.getString("OutputStateTimer.17"); //$NON-NLS-1$
					break;
				case "前車追従未特定": //$NON-NLS-1$
					frontS = Messages.getString("OutputStateTimer.19"); //$NON-NLS-1$
					break;
				case "別車両割り込み": //$NON-NLS-1$
					frontS = Messages.getString("OutputStateTimer.21"); //$NON-NLS-1$
					break;
				case "前車接近追従": //$NON-NLS-1$
					frontS = Messages.getString("OutputStateTimer.23"); //$NON-NLS-1$
					break;
				case "前車離反追従": //$NON-NLS-1$
					frontS = Messages.getString("OutputStateTimer.25"); //$NON-NLS-1$
					break;
	
				default:
					break;
	
				}
				bw.write("," + frontS); //$NON-NLS-1$
	
				// 走行道路
				String routeS = ""; //$NON-NLS-1$
				switch (list.getRoadClass()) {
				case "高速道路": //$NON-NLS-1$
					routeS = Messages.getString("OutputStateTimer.29"); //$NON-NLS-1$
					break;
				case "一般道路": //$NON-NLS-1$
					routeS = Messages.getString("OutputStateTimer.31"); //$NON-NLS-1$
					break;
	
				default:
					break;
	
				}
				bw.write("," + routeS); //$NON-NLS-1$
	
				// 速度
				bw.write("," + list.getSpeed() / 100); //$NON-NLS-1$
				// 車間距離
				bw.write("," + list.getDistance() / 100); //$NON-NLS-1$
				// アクセル開度
				bw.write("," + list.getAccelOpen() / 10); //$NON-NLS-1$
				// 道路種別信号
				bw.write("," + list.getRoadClassSig()); //$NON-NLS-1$
				// 加速度
				if (list.getAccel() == 0x80000000) {
					bw.write("," + Messages.getString("OutputStateTimer.38")); //$NON-NLS-1$ //$NON-NLS-2$
				} else {
					bw.write("," + list.getAccel() / 100); //$NON-NLS-1$
				}
				// TTC
				if (list.getTTC() == 0x7FFFFFFF) {
					bw.write("," + Messages.getString("OutputStateTimer.41")); //$NON-NLS-1$ //$NON-NLS-2$
				} else if (list.getTTC() == 0x80000000) {
					bw.write("," + Messages.getString("OutputStateTimer.43")); //$NON-NLS-1$ //$NON-NLS-2$
				} else {
					bw.write("," + list.getTTC() / 100); //$NON-NLS-1$
				}
				// 処理時間
				bw.write("," + list.getExecutionTime()); //$NON-NLS-1$
	
				// プロファイル1 カウント
				bw.write("," + list.getSuddenAccelCount()); //$NON-NLS-1$
				// プロファイル2 カウント
				bw.write("," + list.getSuddenReduceCount()); //$NON-NLS-1$
				// プロファイル3 カウント
				bw.write("," + list.getReduceStartCount()); //$NON-NLS-1$
				// プロファイル4 カウント
				bw.write("," + list.getTTCRiskCount()); //$NON-NLS-1$
				// プロファイル5 カウント
				bw.write("," + list.getDistanceOnOpenRoadCount()); //$NON-NLS-1$
				// プロファイル6 カウント
				bw.write("," + list.getDistanceOnExpressWayCount()); //$NON-NLS-1$
				// プロファイル7 カウント
				bw.write("," + list.getDistanceofFollowStopCount()); //$NON-NLS-1$
	
				bw.write("\n"); //$NON-NLS-1$
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public void halt() {
		try {
			bw.close();
		} catch (IOException e) {
			// TODO 自動生成された catch ブロック
			e.printStackTrace();
		}
		bw = null;
	}
}
