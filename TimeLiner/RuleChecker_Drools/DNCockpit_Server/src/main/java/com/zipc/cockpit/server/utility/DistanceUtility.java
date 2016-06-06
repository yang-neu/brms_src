package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventDistance;

public class DistanceUtility extends UtilityBase {

	private EventDistance event;
	private KieSession session;
	public DistanceUtility(long value, KieSession session)
	{

		event = new EventDistance(value, "TYPE_DISTANCE.VEHICLE_FOLLOWING_DISTANCE");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		session.getEntryPoint(entryPointName).insert(event);
		
	}
	
}
