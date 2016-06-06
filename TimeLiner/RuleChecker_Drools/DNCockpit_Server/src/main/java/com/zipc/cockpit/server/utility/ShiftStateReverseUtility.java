package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventShiftState;

public class ShiftStateReverseUtility extends UtilityBase {

	private EventShiftState event;
	private KieSession session;
	public ShiftStateReverseUtility(long value, KieSession session)
	{

		event = new EventShiftState((int)value, "TYPE_SHIFT.REVERSE");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		session.getEntryPoint(entryPointName).insert(event);
	}
	
}
