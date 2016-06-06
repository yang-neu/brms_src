package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventAccelOpen;
public class AccelOpenUtility extends UtilityBase {

	private EventAccelOpen event;
	private KieSession session;
	public AccelOpenUtility(long value, KieSession session)
	{

		event = new EventAccelOpen(value, "TYPE_ACCELOPEN.ACCEL_OPEN");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		session.getEntryPoint(entryPointName).insert(event);
	}
	
}
