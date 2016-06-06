package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventRoadClass;

public class RoadClassUtility extends UtilityBase {

	private EventRoadClass event;
	private KieSession session;
	public RoadClassUtility(long value, KieSession session)
	{

		event = new EventRoadClass(value, "TYPE_ROAD_CLASS.ROAD_CLASS");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		
		session.getEntryPoint(entryPointName).insert(event);
	}
	
}
