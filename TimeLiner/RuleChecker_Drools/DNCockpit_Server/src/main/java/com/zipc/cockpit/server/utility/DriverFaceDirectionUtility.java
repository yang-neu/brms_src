package com.zipc.cockpit.server.utility;

import org.kie.api.runtime.KieSession;

import zipc.brms.EventDriverFace;

public class DriverFaceDirectionUtility extends UtilityBase {

	private EventDriverFace event;
	private KieSession session;
	public DriverFaceDirectionUtility(long value, KieSession session)
	{

		event = new EventDriverFace((double)value, "TYPE_FACE.FACE_ANGLE");
		this.session = session;
	}

	@Override
	public void run() {

		String entryPointName = "BRMS Work Stream";
		session.getEntryPoint(entryPointName).insert(event);
	}
	
}
