package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;
import zipc.brms.EventLevelData;

public class AimlessLevelUtility extends UtilityBase {

	private EventLevelData event;
	private KieSession session;
	public AimlessLevelUtility(long value, KieSession session)
	{
		event = new EventLevelData((int)value, "TYPE_LEVEL.AIMLESS_LEVEL");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		session.getEntryPoint(entryPointName).insert(event);
	}
	
}
