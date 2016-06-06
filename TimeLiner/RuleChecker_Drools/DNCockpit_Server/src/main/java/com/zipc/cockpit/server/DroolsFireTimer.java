package com.zipc.cockpit.server;


import java.util.TimerTask;

import org.kie.api.runtime.KieSession;

import com.zipc.cockpit.server.DNCockpitManager.GlobalManager;

public class DroolsFireTimer extends TimerTask{

	private boolean stopflg = false;
	private String agendaName;

	private DNCockpitManager parent;
		
	// Droolsセッション情報
	private KieSession session;
	private GlobalManager globalManager;
		
	public DroolsFireTimer(DNCockpitManager parent, String agendaName, KieSession session)
	{
		this.parent = parent;
		this.agendaName = agendaName;
		this.session = session;
		this.globalManager = parent.getGlobalManager();
	}

	public void halt()
	{
		stopflg = true;
	}
		
	public void run() {  //(2)スレッド実行コードをrunメソッドに記載
        try {
        	if(stopflg)
        	{
        		return;
        	}
        	synchronized(parent.getLockObjectFire()){
        		globalManager.getList().initConsoleLog();
        		session.setGlobal("list", globalManager.getList());
        		session.getAgenda().getAgendaGroup(agendaName).setFocus();
        		long start = System.nanoTime();
        		session.fireAllRules();
        		long end = System.nanoTime();
        		session.getAgenda().getAgendaGroup(agendaName).clear();

        		// 画面情報更新
        		globalManager.getList().setExecutionTime((end-start)/1000);

        		synchronized(parent.getLockObjectOutput()){
        			// グローバルデータのコピー
        			globalManager.getOutput().copyData(globalManager.getList());
                	parent.updateGUI(globalManager.getOutput());
        		}
        	}
        } 
        catch (Exception e) {
        	e.printStackTrace();
            System.exit(1);
        }
	}
}