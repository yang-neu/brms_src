package com.zipc.cockpit.server;

import javax.swing.JFrame;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.UnsupportedEncodingException;

import javax.swing.ButtonGroup;
import javax.swing.JLabel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import zipc.brms.EventOutput;
import java.awt.event.ActionListener;
import javax.swing.SwingConstants;
import java.awt.Font;

public class DNCockpitGUI extends JFrame {
	private static final long serialVersionUID = 1L;

	private DNCockpitManager manager;
    public long starttime = 0;

	JLabel jLabelSpeedData;
	JLabel jLabelAccelerationData;
	JLabel jLabelDistanceData;
	JLabel jLabelAccelOpenData;
	JLabel jLabelTTCData;
	
	private JProgressBar jProgressBarSpeed;
	private JProgressBar jProgressBarAcceleration;
	private JProgressBar jProgressBarDistance;
	private JProgressBar jProgressBarAccelOpen;
	private JProgressBar jProgressBarTTC;
	private JTextField jTextFieldRoadClassSignal;
	private JTextField jTextFieldDriveScene;
	private JTextField jTextFieldFrontCarStat;
	private JTextField jTextFieldRoadClass;
	private JTextField jTextFieldProfile1Specific;
	private JTextField jTextFieldProfile1State;
	private JTextField jTextFieldProfile1SampleCount;
	private JTextField jTextFieldProfile2SampleCount;
	private JTextField jTextFieldProfile2State;
	private JTextField jTextFieldProfile2Specific;
	private JTextField jTextFieldProfile3Specific;
	private JTextField jTextFieldProfile3State;
	private JTextField jTextFieldProfile3SampleCount;
	private JTextField jTextFieldProgile4SampleCount;
	private JTextField jTextFieldProfile4State;
	private JTextField jTextFieldProfile4Specific;
	private JTextField jTextFieldProfile5SampleCount;
	private JTextField jTextFieldProfile5State;
	private JTextField jTextFieldProfile5Specific;
	private JTextField jTextFieldProfile6Specific;
	private JTextField jTextFieldProfile6State;
	private JTextField jTextFieldProfile6SampleCount;
	private JTextField jTextFieldProfile7SampleCount;
	private JTextField jTextFieldProfile7State;
	private JTextField jTextFieldProfile7Specific;
	private JTextField jTextFieldDebugSpeed;
	private JTextField jTextFieldDebugDistance;
	private JTextField jTextFieldDebugAccelOpen;

	private JTextArea jTextArea0;

	JToggleButton jToggleButtonDebug;
	private ButtonGroup group = new ButtonGroup();
	private JRadioButton jRadioButton0;
	private JRadioButton jRadioButton1;
	private JRadioButton jRadioButton2;
	
	JComboBox<String> jComboBoxDebugRoadClassSignal;
	
	public DNCockpitGUI() {
		setSize(new Dimension(1329, 658));
		setTitle(Messages.getString("DNCockpitGUI.0")); //$NON-NLS-1$
		getContentPane().setLayout(null);
		
		JLabel jLabelCanDataMonitor = new JLabel(Messages.getString("DNCockpitGUI.1")); //$NON-NLS-1$
		jLabelCanDataMonitor.setFont(new Font(Messages.getString("DNCockpitGUI.2"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelCanDataMonitor.setBounds(15, 6, 154, 13);
		getContentPane().add(jLabelCanDataMonitor);
		
		JLabel jLabelSpeed = new JLabel(Messages.getString("DNCockpitGUI.3")); //$NON-NLS-1$
		jLabelSpeed.setFont(new Font(Messages.getString("DNCockpitGUI.4"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelSpeed.setBounds(9, 28, 50, 13);
		getContentPane().add(jLabelSpeed);
		
		jLabelSpeedData = new JLabel(Messages.getString("DNCockpitGUI.5")); //$NON-NLS-1$
		jLabelSpeedData.setFont(new Font(Messages.getString("DNCockpitGUI.6"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelSpeedData.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelSpeedData.setBounds(67, 28, 86, 13);
		getContentPane().add(jLabelSpeedData);
		
		JLabel jLabelSpeedUnit = new JLabel(Messages.getString("DNCockpitGUI.7")); //$NON-NLS-1$
		jLabelSpeedUnit.setFont(new Font(Messages.getString("DNCockpitGUI.8"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelSpeedUnit.setBounds(155, 28, 44, 13);
		getContentPane().add(jLabelSpeedUnit);
		
		jProgressBarSpeed = new JProgressBar();
		jProgressBarSpeed.setBounds(23, 47, 146, 14);
		getContentPane().add(jProgressBarSpeed);
		
		JLabel jLabelAcceleration = new JLabel(Messages.getString("DNCockpitGUI.9")); //$NON-NLS-1$
		jLabelAcceleration.setFont(new Font(Messages.getString("DNCockpitGUI.10"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAcceleration.setBounds(9, 74, 76, 13);
		getContentPane().add(jLabelAcceleration);
		
		jLabelAccelerationData = new JLabel(Messages.getString("DNCockpitGUI.11")); //$NON-NLS-1$
		jLabelAccelerationData.setFont(new Font(Messages.getString("DNCockpitGUI.12"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAccelerationData.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelAccelerationData.setBounds(77, 74, 76, 13);
		getContentPane().add(jLabelAccelerationData);
		
		JLabel jLabelAccelerationUnit = new JLabel(Messages.getString("DNCockpitGUI.13")); //$NON-NLS-1$
		jLabelAccelerationUnit.setFont(new Font(Messages.getString("DNCockpitGUI.14"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAccelerationUnit.setBounds(155, 74, 44, 13);
		getContentPane().add(jLabelAccelerationUnit);
		
		jProgressBarAcceleration = new JProgressBar();
		jProgressBarAcceleration.setValue(50);
		jProgressBarAcceleration.setBounds(23, 94, 146, 14);
		getContentPane().add(jProgressBarAcceleration);
		
		JLabel jLabelDistance = new JLabel(Messages.getString("DNCockpitGUI.15")); //$NON-NLS-1$
		jLabelDistance.setFont(new Font(Messages.getString("DNCockpitGUI.16"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDistance.setBounds(9, 118, 113, 13);
		getContentPane().add(jLabelDistance);
		
		jLabelDistanceData = new JLabel(Messages.getString("DNCockpitGUI.17")); //$NON-NLS-1$
		jLabelDistanceData.setFont(new Font(Messages.getString("DNCockpitGUI.18"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDistanceData.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelDistanceData.setBounds(87, 118, 66, 13);
		getContentPane().add(jLabelDistanceData);
		
		JLabel jLabelDistanceUnit = new JLabel(Messages.getString("DNCockpitGUI.19")); //$NON-NLS-1$
		jLabelDistanceUnit.setFont(new Font(Messages.getString("DNCockpitGUI.20"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDistanceUnit.setBounds(155, 118, 44, 13);
		getContentPane().add(jLabelDistanceUnit);
		
		jProgressBarDistance = new JProgressBar();
		jProgressBarDistance.setValue(100);
		jProgressBarDistance.setBounds(23, 137, 146, 14);
		getContentPane().add(jProgressBarDistance);

		JLabel jLabelAccelOpen = new JLabel(Messages.getString("DNCockpitGUI.21")); //$NON-NLS-1$
		jLabelAccelOpen.setFont(new Font(Messages.getString("DNCockpitGUI.22"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAccelOpen.setBounds(9, 167, 125, 13);
		getContentPane().add(jLabelAccelOpen);
		
		jLabelAccelOpenData = new JLabel(Messages.getString("DNCockpitGUI.23")); //$NON-NLS-1$
		jLabelAccelOpenData.setFont(new Font(Messages.getString("DNCockpitGUI.24"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAccelOpenData.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelAccelOpenData.setBounds(117, 167, 36, 13);
		getContentPane().add(jLabelAccelOpenData);
		
		JLabel jLabelAccelOpenUnit = new JLabel(Messages.getString("DNCockpitGUI.25")); //$NON-NLS-1$
		jLabelAccelOpenUnit.setFont(new Font(Messages.getString("DNCockpitGUI.26"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelAccelOpenUnit.setBounds(155, 167, 24, 13);
		getContentPane().add(jLabelAccelOpenUnit);
		
		jProgressBarAccelOpen = new JProgressBar();
		jProgressBarAccelOpen.setBounds(23, 186, 146, 14);
		getContentPane().add(jProgressBarAccelOpen);
		
		JLabel jLabelTTCUnit = new JLabel(Messages.getString("DNCockpitGUI.27")); //$NON-NLS-1$
		jLabelTTCUnit.setFont(new Font(Messages.getString("DNCockpitGUI.28"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelTTCUnit.setBounds(155, 210, 44, 13);
		getContentPane().add(jLabelTTCUnit);

		JLabel jLabelTTC = new JLabel(Messages.getString("DNCockpitGUI.29")); //$NON-NLS-1$
		jLabelTTC.setFont(new Font(Messages.getString("DNCockpitGUI.30"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelTTC.setBounds(9, 210, 66, 13);
		getContentPane().add(jLabelTTC);

		jLabelTTCData = new JLabel(Messages.getString("DNCockpitGUI.31")); //$NON-NLS-1$
		jLabelTTCData.setFont(new Font(Messages.getString("DNCockpitGUI.32"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelTTCData.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelTTCData.setBounds(67, 210, 86, 13);
		getContentPane().add(jLabelTTCData);

		jProgressBarTTC = new JProgressBar();
		jProgressBarTTC.setBounds(23, 229, 146, 14);
		getContentPane().add(jProgressBarTTC);

		JLabel jLabelRoadClassSignal = new JLabel(Messages.getString("DNCockpitGUI.33"));
		jLabelRoadClassSignal.setFont(new Font(Messages.getString("DNCockpitGUI.34"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelRoadClassSignal.setBounds(9, 255, 180, 13);
		getContentPane().add(jLabelRoadClassSignal);
		
		jTextFieldRoadClassSignal = new JTextField();
		jTextFieldRoadClassSignal.setBounds(23, 277, 146, 24);
		getContentPane().add(jTextFieldRoadClassSignal);
		jTextFieldRoadClassSignal.setColumns(10);
		
		JLabel jLabelCanDataAnalysis = new JLabel(Messages.getString("DNCockpitGUI.35")); //$NON-NLS-1$
		jLabelCanDataAnalysis.setFont(new Font(Messages.getString("DNCockpitGUI.36"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelCanDataAnalysis.setBounds(184, 6, 129, 13);
		getContentPane().add(jLabelCanDataAnalysis);
		
		JLabel jLabelDriveScene = new JLabel(Messages.getString("DNCockpitGUI.37")); //$NON-NLS-1$
		jLabelDriveScene.setFont(new Font(Messages.getString("DNCockpitGUI.38"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDriveScene.setBounds(194, 48, 96, 13);
		getContentPane().add(jLabelDriveScene);
		
		jTextFieldDriveScene = new JTextField();
		jTextFieldDriveScene.setFont(new Font(Messages.getString("DNCockpitGUI.39"), Font.PLAIN, 12)); //$NON-NLS-1$
		jTextFieldDriveScene.setText(Messages.getString("DNCockpitGUI.40")); //$NON-NLS-1$
		jTextFieldDriveScene.setColumns(10);
		jTextFieldDriveScene.setBounds(212, 68, 91, 26);
		getContentPane().add(jTextFieldDriveScene);
		
		JLabel jLabelFrontCarStat = new JLabel(Messages.getString("DNCockpitGUI.41")); //$NON-NLS-1$
		jLabelFrontCarStat.setFont(new Font(Messages.getString("DNCockpitGUI.42"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelFrontCarStat.setBounds(194, 118, 119, 13);
		getContentPane().add(jLabelFrontCarStat);
		
		jTextFieldFrontCarStat = new JTextField();
		jTextFieldFrontCarStat.setText(Messages.getString("DNCockpitGUI.43")); //$NON-NLS-1$
		jTextFieldFrontCarStat.setColumns(10);
		jTextFieldFrontCarStat.setBounds(212, 136, 91, 26);
		getContentPane().add(jTextFieldFrontCarStat);
		
		jTextFieldRoadClass = new JTextField();
		jTextFieldRoadClass.setText(Messages.getString("DNCockpitGUI.44")); //$NON-NLS-1$
		jTextFieldRoadClass.setColumns(10);
		jTextFieldRoadClass.setBounds(212, 206, 91, 26);
		getContentPane().add(jTextFieldRoadClass);
		
		JLabel jLabelRoadClass = new JLabel(Messages.getString("DNCockpitGUI.45")); //$NON-NLS-1$
		jLabelRoadClass.setFont(new Font(Messages.getString("DNCockpitGUI.46"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelRoadClass.setBounds(194, 184, 96, 13);
		getContentPane().add(jLabelRoadClass);
		
		JLabel jLabelProfile = new JLabel(Messages.getString("DNCockpitGUI.47")); //$NON-NLS-1$
		jLabelProfile.setFont(new Font(Messages.getString("DNCockpitGUI.48"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile.setBounds(325, 6, 313, 13);
		getContentPane().add(jLabelProfile);
		
		JLabel jLabelSampleCount = new JLabel(Messages.getString("DNCockpitGUI.49")); //$NON-NLS-1$
		jLabelSampleCount.setHorizontalAlignment(SwingConstants.TRAILING);
		jLabelSampleCount.setFont(new Font(Messages.getString("DNCockpitGUI.50"), Font.BOLD, 12));
		jLabelSampleCount.setBounds(585, 52, 123, 13);
		getContentPane().add(jLabelSampleCount);
		
		JLabel jLabelProfile1 = new JLabel(Messages.getString("DNCockpitGUI.51")); //$NON-NLS-1$
		jLabelProfile1.setFont(new Font(Messages.getString("DNCockpitGUI.52"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile1.setBounds(317, 48, 270, 20);
		getContentPane().add(jLabelProfile1);
		
		jTextFieldProfile1Specific = new JTextField();
		jTextFieldProfile1Specific.setText(Messages.getString("DNCockpitGUI.53")); //$NON-NLS-1$
		jTextFieldProfile1Specific.setColumns(10);
		jTextFieldProfile1Specific.setBounds(320, 68, 196, 24);
		getContentPane().add(jTextFieldProfile1Specific);
		
		jTextFieldProfile1State = new JTextField();
		jTextFieldProfile1State.setText(Messages.getString("DNCockpitGUI.54")); //$NON-NLS-1$
		jTextFieldProfile1State.setColumns(10);
		jTextFieldProfile1State.setBounds(526, 68, 130, 24);
		getContentPane().add(jTextFieldProfile1State);
		
		jTextFieldProfile1SampleCount = new JTextField();
		jTextFieldProfile1SampleCount.setText(Messages.getString("DNCockpitGUI.55")); //$NON-NLS-1$
		jTextFieldProfile1SampleCount.setColumns(10);
		jTextFieldProfile1SampleCount.setBounds(666, 68, 36, 24);
		getContentPane().add(jTextFieldProfile1SampleCount);
		
		JLabel jLabelProfile2 = new JLabel(Messages.getString("DNCockpitGUI.56")); //$NON-NLS-1$
		jLabelProfile2.setFont(new Font(Messages.getString("DNCockpitGUI.57"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile2.setBounds(317, 94, 270, 20);
		getContentPane().add(jLabelProfile2);
		
		jTextFieldProfile2Specific = new JTextField();
		jTextFieldProfile2Specific.setText(Messages.getString("DNCockpitGUI.58")); //$NON-NLS-1$
		jTextFieldProfile2Specific.setColumns(10);
		jTextFieldProfile2Specific.setBounds(320, 114, 196, 24);
		getContentPane().add(jTextFieldProfile2Specific);

		jTextFieldProfile2State = new JTextField();
		jTextFieldProfile2State.setText(Messages.getString("DNCockpitGUI.59")); //$NON-NLS-1$
		jTextFieldProfile2State.setColumns(10);
		jTextFieldProfile2State.setBounds(526, 114, 130, 24);
		getContentPane().add(jTextFieldProfile2State);
		
		jTextFieldProfile2SampleCount = new JTextField();
		jTextFieldProfile2SampleCount.setText(Messages.getString("DNCockpitGUI.60")); //$NON-NLS-1$
		jTextFieldProfile2SampleCount.setColumns(10);
		jTextFieldProfile2SampleCount.setBounds(666, 114, 36, 24);
		getContentPane().add(jTextFieldProfile2SampleCount);

		JLabel jLabelProfile3 = new JLabel(Messages.getString("DNCockpitGUI.61")); //$NON-NLS-1$
		jLabelProfile3.setFont(new Font(Messages.getString("DNCockpitGUI.62"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile3.setBounds(317, 147, 339, 20);
		getContentPane().add(jLabelProfile3);
		
		jTextFieldProfile3Specific = new JTextField();
		jTextFieldProfile3Specific.setText(Messages.getString("DNCockpitGUI.63")); //$NON-NLS-1$
		jTextFieldProfile3Specific.setColumns(10);
		jTextFieldProfile3Specific.setBounds(320, 167, 196, 24);
		getContentPane().add(jTextFieldProfile3Specific);
		
		jTextFieldProfile3State = new JTextField();
		jTextFieldProfile3State.setText(Messages.getString("DNCockpitGUI.64")); //$NON-NLS-1$
		jTextFieldProfile3State.setColumns(10);
		jTextFieldProfile3State.setBounds(526, 167, 130, 24);
		getContentPane().add(jTextFieldProfile3State);
		
		jTextFieldProfile3SampleCount = new JTextField();
		jTextFieldProfile3SampleCount.setText(Messages.getString("DNCockpitGUI.65")); //$NON-NLS-1$
		jTextFieldProfile3SampleCount.setColumns(10);
		jTextFieldProfile3SampleCount.setBounds(666, 167, 36, 24);
		getContentPane().add(jTextFieldProfile3SampleCount);

		JLabel jLabelProfile4 = new JLabel(Messages.getString("DNCockpitGUI.66")); //$NON-NLS-1$
		jLabelProfile4.setFont(new Font(Messages.getString("DNCockpitGUI.67"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile4.setBounds(317, 195, 339, 20);
		getContentPane().add(jLabelProfile4);

		jTextFieldProfile4Specific = new JTextField();
		jTextFieldProfile4Specific.setText(Messages.getString("DNCockpitGUI.68")); //$NON-NLS-1$
		jTextFieldProfile4Specific.setColumns(10);
		jTextFieldProfile4Specific.setBounds(320, 215, 196, 24);
		getContentPane().add(jTextFieldProfile4Specific);
		
		jTextFieldProfile4State = new JTextField();
		jTextFieldProfile4State.setText(Messages.getString("DNCockpitGUI.69")); //$NON-NLS-1$
		jTextFieldProfile4State.setColumns(10);
		jTextFieldProfile4State.setBounds(526, 215, 130, 24);
		getContentPane().add(jTextFieldProfile4State);

		jTextFieldProgile4SampleCount = new JTextField();
		jTextFieldProgile4SampleCount.setText(Messages.getString("DNCockpitGUI.70")); //$NON-NLS-1$
		jTextFieldProgile4SampleCount.setColumns(10);
		jTextFieldProgile4SampleCount.setBounds(666, 215, 36, 24);
		getContentPane().add(jTextFieldProgile4SampleCount);
		
		JLabel jLabelProfile5 = new JLabel(Messages.getString("DNCockpitGUI.71")); //$NON-NLS-1$
		jLabelProfile5.setFont(new Font(Messages.getString("DNCockpitGUI.72"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile5.setBounds(317, 249, 339, 20);
		getContentPane().add(jLabelProfile5);

		jTextFieldProfile5Specific = new JTextField();
		jTextFieldProfile5Specific.setText(Messages.getString("DNCockpitGUI.73")); //$NON-NLS-1$
		jTextFieldProfile5Specific.setColumns(10);
		jTextFieldProfile5Specific.setBounds(320, 269, 196, 24);
		getContentPane().add(jTextFieldProfile5Specific);

		jTextFieldProfile5State = new JTextField();
		jTextFieldProfile5State.setText(Messages.getString("DNCockpitGUI.74")); //$NON-NLS-1$
		jTextFieldProfile5State.setColumns(10);
		jTextFieldProfile5State.setBounds(526, 269, 130, 24);
		getContentPane().add(jTextFieldProfile5State);
		
		jTextFieldProfile5SampleCount = new JTextField();
		jTextFieldProfile5SampleCount.setText(Messages.getString("DNCockpitGUI.75")); //$NON-NLS-1$
		jTextFieldProfile5SampleCount.setColumns(10);
		jTextFieldProfile5SampleCount.setBounds(666, 269, 36, 24);
		getContentPane().add(jTextFieldProfile5SampleCount);

		JLabel jLabelProfile6 = new JLabel(Messages.getString("DNCockpitGUI.76")); //$NON-NLS-1$
		jLabelProfile6.setFont(new Font(Messages.getString("DNCockpitGUI.77"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile6.setBounds(317, 300, 339, 20);
		getContentPane().add(jLabelProfile6);
		
		jTextFieldProfile6Specific = new JTextField();
		jTextFieldProfile6Specific.setText(Messages.getString("DNCockpitGUI.78")); //$NON-NLS-1$
		jTextFieldProfile6Specific.setColumns(10);
		jTextFieldProfile6Specific.setBounds(320, 320, 196, 24);
		getContentPane().add(jTextFieldProfile6Specific);
		
		jTextFieldProfile6State = new JTextField();
		jTextFieldProfile6State.setText(Messages.getString("DNCockpitGUI.79")); //$NON-NLS-1$
		jTextFieldProfile6State.setColumns(10);
		jTextFieldProfile6State.setBounds(526, 320, 130, 24);
		getContentPane().add(jTextFieldProfile6State);
		
		jTextFieldProfile6SampleCount = new JTextField();
		jTextFieldProfile6SampleCount.setText(Messages.getString("DNCockpitGUI.80")); //$NON-NLS-1$
		jTextFieldProfile6SampleCount.setColumns(10);
		jTextFieldProfile6SampleCount.setBounds(666, 320, 36, 24);
		getContentPane().add(jTextFieldProfile6SampleCount);
		
		jTextFieldProfile7SampleCount = new JTextField();
		JLabel jLabelProfile7 = new JLabel(Messages.getString("DNCockpitGUI.81")); //$NON-NLS-1$
		jLabelProfile7.setFont(new Font(Messages.getString("DNCockpitGUI.82"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelProfile7.setBounds(317, 350, 339, 20);
		getContentPane().add(jLabelProfile7);
		
		jTextFieldProfile7Specific = new JTextField();
		jTextFieldProfile7Specific.setText(Messages.getString("DNCockpitGUI.83")); //$NON-NLS-1$
		jTextFieldProfile7Specific.setColumns(10);
		jTextFieldProfile7Specific.setBounds(320, 370, 196, 24);
		getContentPane().add(jTextFieldProfile7Specific);
		
		jTextFieldProfile7State = new JTextField();
		jTextFieldProfile7State.setText(Messages.getString("DNCockpitGUI.84")); //$NON-NLS-1$
		jTextFieldProfile7State.setColumns(10);
		jTextFieldProfile7State.setBounds(526, 370, 130, 24);
		getContentPane().add(jTextFieldProfile7State);
		
		jTextFieldProfile7SampleCount.setText(Messages.getString("DNCockpitGUI.85")); //$NON-NLS-1$
		jTextFieldProfile7SampleCount.setColumns(10);
		jTextFieldProfile7SampleCount.setBounds(666, 370, 36, 24);
		getContentPane().add(jTextFieldProfile7SampleCount);
		
		JSeparator jSeparator0 = new JSeparator();
		jSeparator0.setBounds(11, 412, 679, 10);
		getContentPane().add(jSeparator0);
		
		JLabel jLabelDebug = new JLabel(Messages.getString("DNCockpitGUI.86")); //$NON-NLS-1$
		jLabelDebug.setFont(new Font(Messages.getString("DNCockpitGUI.87"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDebug.setBounds(23, 426, 86, 24);
		getContentPane().add(jLabelDebug);
		
		jToggleButtonDebug = new JToggleButton(Messages.getString("DNCockpitGUI.88")); //$NON-NLS-1$
		jToggleButtonDebug.setFont(new Font(Messages.getString("DNCockpitGUI.89"), Font.BOLD, 12)); //$NON-NLS-1$
		jToggleButtonDebug.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jToggleButton0ActionActionPerformed(e);
			}
		});
		jToggleButtonDebug.setBounds(19, 455, 86, 58);
		getContentPane().add(jToggleButtonDebug);
		
		JButton jButtonDebugSpeed = new JButton(Messages.getString("DNCockpitGUI.90")); //$NON-NLS-1$
		jButtonDebugSpeed.setFont(new Font(Messages.getString("DNCockpitGUI.91"), Font.BOLD, 12)); //$NON-NLS-1$
		jButtonDebugSpeed.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jButton0ActionActionPerformed(e);
			}
		});
		jButtonDebugSpeed.setBounds(117, 435, 173, 29);
		getContentPane().add(jButtonDebugSpeed);
		
		jTextFieldDebugSpeed = new JTextField();
		jTextFieldDebugSpeed.setText(Messages.getString("DNCockpitGUI.92")); //$NON-NLS-1$
		jTextFieldDebugSpeed.setColumns(10);
		jTextFieldDebugSpeed.setBounds(309, 438, 84, 24);
		getContentPane().add(jTextFieldDebugSpeed);
		
		JLabel jLabelDebugSpeedUnit = new JLabel(Messages.getString("DNCockpitGUI.93")); //$NON-NLS-1$
		jLabelDebugSpeedUnit.setFont(new Font(Messages.getString("DNCockpitGUI.94"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDebugSpeedUnit.setBounds(407, 440, 50, 12);
		getContentPane().add(jLabelDebugSpeedUnit);
		
		JButton jButtonDebugDistance = new JButton(Messages.getString("DNCockpitGUI.95")); //$NON-NLS-1$
		jButtonDebugDistance.setFont(new Font(Messages.getString("DNCockpitGUI.96"), Font.BOLD, 12)); //$NON-NLS-1$
		jButtonDebugDistance.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jButton1ActionActionPerformed(e);
				}
		});
		jButtonDebugDistance.setBounds(117, 479, 173, 29);
		getContentPane().add(jButtonDebugDistance);
		
		jTextFieldDebugDistance = new JTextField();
		jTextFieldDebugDistance.setText(Messages.getString("DNCockpitGUI.97")); //$NON-NLS-1$
		jTextFieldDebugDistance.setColumns(10);
		jTextFieldDebugDistance.setBounds(309, 484, 84, 24);
		getContentPane().add(jTextFieldDebugDistance);
		
		JLabel jLabelDebugDistanceUnit = new JLabel(Messages.getString("DNCockpitGUI.98")); //$NON-NLS-1$
		jLabelDebugDistanceUnit.setFont(new Font(Messages.getString("DNCockpitGUI.99"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDebugDistanceUnit.setBounds(406, 482, 50, 12);
		getContentPane().add(jLabelDebugDistanceUnit);
		
		JButton jButtonDebugAccelOpen = new JButton(Messages.getString("DNCockpitGUI.100")); //$NON-NLS-1$
		jButtonDebugAccelOpen.setFont(new Font(Messages.getString("DNCockpitGUI.101"), Font.BOLD, 12)); //$NON-NLS-1$
		jButtonDebugAccelOpen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jButton2ActionActionPerformed(e);
			}
		});
		jButtonDebugAccelOpen.setBounds(117, 524, 173, 29);
		getContentPane().add(jButtonDebugAccelOpen);
		
		jTextFieldDebugAccelOpen = new JTextField();
		jTextFieldDebugAccelOpen.setText(Messages.getString("DNCockpitGUI.102")); //$NON-NLS-1$
		jTextFieldDebugAccelOpen.setColumns(10);
		jTextFieldDebugAccelOpen.setBounds(309, 527, 84, 24);
		getContentPane().add(jTextFieldDebugAccelOpen);
		
		JLabel jLabelDebugAccelOpenUnit = new JLabel(Messages.getString("DNCockpitGUI.103")); //$NON-NLS-1$
		jLabelDebugAccelOpenUnit.setFont(new Font(Messages.getString("DNCockpitGUI.104"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelDebugAccelOpenUnit.setBounds(407, 524, 50, 20);
		getContentPane().add(jLabelDebugAccelOpenUnit);
		
		JButton jButtonDebugRoadClassSignal = new JButton(Messages.getString("DNCockpitGUI.105")); //$NON-NLS-1$
		jButtonDebugRoadClassSignal.setFont(new Font(Messages.getString("DNCockpitGUI.106"), Font.BOLD, 12)); //$NON-NLS-1$
		jButtonDebugRoadClassSignal.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jButton4ActionActionPerformed(e);
			}
		});
		jButtonDebugRoadClassSignal.setBounds(117, 568, 173, 29);
		getContentPane().add(jButtonDebugRoadClassSignal);
		
		jComboBoxDebugRoadClassSignal = new JComboBox<String>();
		jComboBoxDebugRoadClassSignal.setFont(new Font(Messages.getString("DNCockpitGUI.107"), Font.BOLD, 12)); //$NON-NLS-1$
		jComboBoxDebugRoadClassSignal.setModel(new DefaultComboBoxModel<String>(new String[] {Messages.getString("DNCockpitGUI.108"), Messages.getString("DNCockpitGUI.109"), Messages.getString("DNCockpitGUI.110"), Messages.getString("DNCockpitGUI.111"), Messages.getString("DNCockpitGUI.112"), Messages.getString("DNCockpitGUI.113"), Messages.getString("DNCockpitGUI.114")})); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$
		jComboBoxDebugRoadClassSignal.setBounds(309, 570, 207, 24);
		getContentPane().add(jComboBoxDebugRoadClassSignal);
		
		jRadioButton0 = new JRadioButton(Messages.getString("DNCockpitGUI.115")); //$NON-NLS-1$
		jRadioButton0.setFont(new Font(Messages.getString("DNCockpitGUI.116"), Font.BOLD, 12)); //$NON-NLS-1$
		jRadioButton0.setSelected(true);
		jRadioButton0.setBounds(474, 443, 113, 21);
		getContentPane().add(jRadioButton0);
		
		jRadioButton1 = new JRadioButton(Messages.getString("DNCockpitGUI.117")); //$NON-NLS-1$
		jRadioButton1.setFont(new Font(Messages.getString("DNCockpitGUI.118"), Font.BOLD, 12)); //$NON-NLS-1$
		jRadioButton1.setBounds(474, 474, 154, 21);
		getContentPane().add(jRadioButton1);
		
		jRadioButton2 = new JRadioButton(Messages.getString("DNCockpitGUI.119")); //$NON-NLS-1$
		jRadioButton2.setFont(new Font(Messages.getString("DNCockpitGUI.120"), Font.BOLD, 12)); //$NON-NLS-1$
		jRadioButton2.setBounds(474, 504, 154, 21);
		getContentPane().add(jRadioButton2);
		
		JLabel jLabelSave = new JLabel(Messages.getString("DNCockpitGUI.121")); //$NON-NLS-1$
		jLabelSave.setFont(new Font(Messages.getString("DNCockpitGUI.122"), Font.BOLD, 12)); //$NON-NLS-1$
		jLabelSave.setBounds(600, 536, 117, 13);
		getContentPane().add(jLabelSave);
		
		JButton jButtonIGOFF = new JButton(Messages.getString("DNCockpitGUI.123")); //$NON-NLS-1$
		jButtonIGOFF.setFont(new Font(Messages.getString("DNCockpitGUI.124"), Font.BOLD, 12)); //$NON-NLS-1$
		jButtonIGOFF.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jButton3ActionActionPerformed(e);
			}
		});
		jButtonIGOFF.setBounds(614, 562, 71, 29);
		getContentPane().add(jButtonIGOFF);
		
		JScrollPane jScrollPane0 = new JScrollPane();
		jScrollPane0.setBounds(716, 26, 588, 562);
		getContentPane().add(jScrollPane0);
		
		jTextArea0 = new JTextArea();
		jTextArea0.setText(Messages.getString("DNCockpitGUI.125")); //$NON-NLS-1$
		jScrollPane0.setViewportView(jTextArea0);

		addWindowListener(new WindowAdapter() {
			
			public void windowClosing(WindowEvent event) {
				windowWindowClosing(event);
			}
		});
		windowWindowOpened();
	}

	public void updateDisplay(EventOutput data)
	{
		String utf8str = data.getConsoleLog();
		byte[] buf;
		try {
			buf = utf8str.getBytes(Messages.getString("DNCockpitGUI.126")); //$NON-NLS-1$
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return;
		}
		String ms932str;
		try {
			ms932str = new String(buf, Messages.getString("DNCockpitGUI.127")); //$NON-NLS-1$
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return;
		}
		jTextArea0.append(ms932str);
	}

	// ロックオブジェクト
	static public Object GUIlockObject = new Object();
	
	private String debugSpeed = Messages.getString("DNCockpitGUI.128"); //$NON-NLS-1$
	private String debugDistance = Messages.getString("DNCockpitGUI.129");  //$NON-NLS-1$
	private String debugAccelOpen = Messages.getString("DNCockpitGUI.130"); //$NON-NLS-1$
	private int debugRoadClass = 0;
	private int debugDistanceMove = 0;
	private double delta=0.0;

	// ウインドウ終了時の処理
	private void windowWindowClosing(WindowEvent event) {
		System.out.println(Messages.getString("DNCockpitGUI.131")); //$NON-NLS-1$
		try
		{
			manager.halt();
			manager = null;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{

			System.out.println(Messages.getString("DNCockpitGUI.132")); //$NON-NLS-1$
			System.exit(0);
		}
		//dispose();
	}

	private void windowWindowOpened() {
		manager = new DNCockpitManager(this);
		
		debugSpeed = jTextFieldDebugSpeed.getText();
		debugDistance = jTextFieldDebugDistance.getText();
		debugAccelOpen = jTextFieldDebugAccelOpen.getText();
		
		// ボタングループの設定
		group.add(jRadioButton0);
		group.add(jRadioButton1);
		group.add(jRadioButton2);
		
		// 加速度真ん中
		jProgressBarAcceleration.setValue(50);
		// 車間距離最大
		jProgressBarDistance.setValue(100);
	}
	
	// デバッグ機能
	public String getSpeed()
	{
		return debugSpeed;
	}

	public String getDistance()
	{
		String retStr;
		if(debugDistanceMove == 0)
		{
			delta = 0.0;
			retStr =  debugDistance; 
		}
		else if(debugDistanceMove == 1)
		{
			double distance = Double.valueOf(debugDistance);
			delta = delta-0.025;
			if( distance+delta >= 0)
			{
				retStr = String.format(Messages.getString("DNCockpitGUI.133"),distance+Math.floor(delta)); //$NON-NLS-1$
			}
			else
			{
				delta = delta+0.075;
				retStr = Messages.getString("DNCockpitGUI.134"); //$NON-NLS-1$
			}
			jTextFieldDebugDistance.setText(retStr);
			repaint();
		}
		else if(debugDistanceMove == 2)
		{
			delta = delta+0.025;
			double distance = Double.valueOf(debugDistance);
			if(( distance+delta <= 254) && ( distance+delta >= 0))
			{
				retStr = String.format(Messages.getString("DNCockpitGUI.135"),distance+Math.floor(delta)); //$NON-NLS-1$
			}
			else if( distance+delta > 254)
			{
				delta = delta-0.025;
				retStr =  Messages.getString("DNCockpitGUI.136"); //$NON-NLS-1$
			}
			else 
			{
				delta = delta+0.025;
				retStr =  Messages.getString("DNCockpitGUI.137"); //$NON-NLS-1$
			}
			jTextFieldDebugDistance.setText(retStr);
			repaint();
		}
		else{
			retStr = debugDistance; 
			jTextFieldDebugDistance.setText(retStr);
			repaint();
		}
		return retStr;
	}
	public String getAccelOpen()
	{
		return debugAccelOpen;
	}
	public Long getRoadClass() {
	
		switch(debugRoadClass)
		{
		case 0:
			return (long)2;
		case 1:
			return (long)3;
		case 2:
			return (long)4;
		case 3:
			return (long)5;
		case 4:
			return (long)6;
		case 5:
			return (long)0;
		case 6:
			return (long)1;
		default:
			return (long)0;
		}
	}

	// ロック参照取得
	public Object getGUILockObject()
	{
		return GUIlockObject;
	}

    // 車速変更
	private void jButton0ActionActionPerformed(ActionEvent event) {
		debugSpeed = jTextFieldDebugSpeed.getText();
	}
	// 車間距離変更
	private void jButton1ActionActionPerformed(ActionEvent event) {
		if(jRadioButton0.isSelected())
		{
			debugDistanceMove = 0;
		}
		else if(jRadioButton1.isSelected())
		{
			debugDistanceMove = 1;
		}
		else if(jRadioButton2.isSelected())
		{
			debugDistanceMove = 2;
		}
		debugDistance = jTextFieldDebugDistance.getText();
	}
	// アクセル開度変更
	private void jButton2ActionActionPerformed(ActionEvent event) {
		debugAccelOpen = jTextFieldDebugAccelOpen.getText();
	}

	// 走行道路変更
	private void jButton4ActionActionPerformed(ActionEvent event) {
		debugRoadClass = jComboBoxDebugRoadClassSignal.getSelectedIndex();
	}
	
	// デバッグモード有効/無効設定
	private void jToggleButton0ActionActionPerformed(ActionEvent event) {
		if(jToggleButtonDebug.getText().equals(Messages.getString("DNCockpitGUI.138"))) //$NON-NLS-1$
		{
			jToggleButtonDebug.setText(Messages.getString("DNCockpitGUI.139")); //$NON-NLS-1$
			manager.runDebugMode();
		}
		else
		{
			jToggleButtonDebug.setText(Messages.getString("DNCockpitGUI.140")); //$NON-NLS-1$
			manager.stopDebugMode();
		}
	}

	// outputの内容で画面情報を更新する
	public void updateGUI(EventOutput output) {
		// 急加速頻度
		jTextFieldProfile1Specific.setText(output.getSuddenAccel());
		jTextFieldProfile1State.setText(output.getSuddenAccelState());
		jTextFieldProfile1SampleCount.setText(String.valueOf(output.getSuddenAccelCount()));
		// 急減速頻度
		jTextFieldProfile2Specific.setText(output.getSuddenReduce());
		jTextFieldProfile2State.setText(output.getSuddenReduceState());
		jTextFieldProfile2SampleCount.setText(String.valueOf(output.getSuddenReduceCount()));
		// 他車追従時の減速開始タイミング
		jTextFieldProfile3Specific.setText(output.getReduceStart());
		jTextFieldProfile3State.setText(output.getReduceStartState());
		jTextFieldProfile3SampleCount.setText(String.valueOf(output.getReduceStartCount()));
		// ＴＴＣ短時間へのリスク
		jTextFieldProfile4Specific.setText(output.getTTCRisk());
		jTextFieldProfile4State.setText(output.getTTCRiskState());
		jTextFieldProgile4SampleCount.setText(String.valueOf(output.getTTCRiskCount()));
		// 一般道路走行時の平均車間距離
		jTextFieldProfile5Specific.setText(output.getDistanceOnOpenRoad());
		jTextFieldProfile5State.setText(output.getDistanceOnOpenRoadState());
		jTextFieldProfile5SampleCount.setText(String.valueOf(output.getDistanceOnOpenRoadCount()));
		// 高速道路走行時の平均車間距離
		jTextFieldProfile6Specific.setText(output.getDistanceOnExpressWay());
		jTextFieldProfile6State.setText(output.getDistanceOnExpressWayState());
		jTextFieldProfile6SampleCount.setText(String.valueOf(output.getDistanceOnExpressWayCount()));
		// 追従停車時の平均車間距離
		jTextFieldProfile7Specific.setText(output.getDistanceofFollowStop());
		jTextFieldProfile7State.setText(output.getDistanceofFollowStopState());
		jTextFieldProfile7SampleCount.setText(String.valueOf(output.getDistanceofFollowStopCount()));
		
		// 車速(0.01km/hを1km/hに単位変更)
		jProgressBarSpeed.setValue((int)(output.getSpeed()/100.0));
		jLabelSpeedData.setText(String.format(Messages.getString("DNCockpitGUI.141"),output.getSpeed()/100.0)); //$NON-NLS-1$
		// 加速度
		if((int)output.getAccel() == 0x80000000) {
			jProgressBarAcceleration.setValue(0);
			jLabelAccelerationData.setText(Messages.getString("DNCockpitGUI.142")); //$NON-NLS-1$
		} else {
			jProgressBarAcceleration.setValue((int)output.getAccel()+50);
			jLabelAccelerationData.setText(String.format(Messages.getString("DNCockpitGUI.143"),(output.getAccel()/100))); //$NON-NLS-1$
		}
		// 車間距離(0.01mを1mに単位変更)
		if(output.getDistance() == 25500) {
			jProgressBarDistance.setValue(0);
			jLabelDistanceData.setText(Messages.getString("DNCockpitGUI.144"));			 //$NON-NLS-1$
		}else {
			jProgressBarDistance.setValue((int)(output.getDistance()/100.0));
			jLabelDistanceData.setText(String.format(Messages.getString("DNCockpitGUI.145"),output.getDistance()/100.0)); //$NON-NLS-1$
		}
		// TTC
		if((int)output.getTTC() == 0x7FFFFFFF) {
			jProgressBarTTC.setValue(100);
			jLabelTTCData.setText(Messages.getString("DNCockpitGUI.146")); //$NON-NLS-1$
		}
		else if((int)output.getTTC() == 0x80000000) {
			jProgressBarTTC.setValue(0);
			jLabelTTCData.setText(Messages.getString("DNCockpitGUI.147")); //$NON-NLS-1$
		}
		else {
			jProgressBarTTC.setValue((int)output.getTTC());
			jLabelTTCData.setText(String.format(Messages.getString("DNCockpitGUI.148"),output.getTTC())); //$NON-NLS-1$
		}
		// アクセル開度(0.1%を1%に単位変更)
		jProgressBarAccelOpen.setValue((int)(output.getAccelOpen()/10.0));
		jLabelAccelOpenData.setText(String.format(Messages.getString("DNCockpitGUI.149"),output.getAccelOpen()/10.0)); //$NON-NLS-1$
		// 道路種別判定信号
		jTextFieldRoadClassSignal.setText(output.getRoadClassSig());
		
		// 運転シーン
		jTextFieldDriveScene.setText(output.getDriveScene());
		// 前車有無シーン
		jTextFieldFrontCarStat.setText(output.getWithFrontCar());
		// 前車有無シーン
		jTextFieldRoadClass.setText(output.getRoadClass());
		
		jTextArea0.append(output.getConsoleLog());
		
		// 際描画
		repaint();
	}

	// ユーザ情報保存ボタン押下
	private void jButton3ActionActionPerformed(ActionEvent event) {

	    // ユーザ特性ファイル読み込み
		manager.writeUserSpecificDataFile();
	}
}
