package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventSpeed;

public class SpeedUtility extends UtilityBase {

	private EventSpeed event;
	private KieSession session;
	public SpeedUtility(long value, KieSession session)
	{

		event = new EventSpeed(value, "TYPE_SPEED.VEHICLE_SPEED_SP1");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		
		session.getEntryPoint(entryPointName).insert(event);
		
	}
	
}
