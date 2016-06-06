package com.zipc.cockpit.server;

import java.util.List;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Iterator;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

import zipc.brms.EventOutput;

class TimelinerConfigItem {
	public int no;
	public String member;
	public String comment;
	public String type;
	public String method;
	public String kind;
	public String calculate;
	public boolean output;
	public int order;
	public boolean diagram;
	public boolean judge;

	private TimelinerConfigItem() {

	}

	public String format(String val, ScriptEngine se) throws ScriptException {
		if (calculate.equals("")) {
			return val;
		}
		
		String str = "val = '" + val + "';" + calculate + "; '' + val;";
		
		String result =(String) se.eval(str);

		return result;
	}

	public static TimelinerConfigItem ParseConfigItem(String line) {
		String items[] = line.split(",");
		if (items.length < 8 || items[0].equals("No") || Integer.parseInt(items[0]) <= 0) {
			return null;
		}

		TimelinerConfigItem tci = new TimelinerConfigItem();

		tci.no = Integer.parseInt(items[0]);
		tci.member = items[1];
		tci.comment = items[2];
		tci.type = items[3];
		tci.method = items[4];
		tci.kind = items[5];
		tci.calculate = items[6];
		tci.output = items[7].equals("レ");
		tci.order = items.length > 8 ? Integer.parseInt(items[8]) : 0;
		tci.diagram = items.length > 9 ? items[9].equals("レ") : false;
		tci.judge = items.length > 10 ? items[10].equals("レ") : false;

		return tci;
	}
}

public class TimelinerConfig {
	private List<TimelinerConfigItem> list;
	private ScriptEngine se;

	private TimelinerConfig() {

	}

	public void printHeader(BufferedWriter writer) throws IOException {
		Iterator<TimelinerConfigItem> iter = list.iterator();
		int i = 0;
		while (iter.hasNext()) {
			writer.write((i > 0 ? "," : "") + iter.next().member);
			i++;
		}
		writer.write("\n");
	}

	public void printData(EventOutput output, BufferedWriter writer) throws NoSuchMethodException, SecurityException,
			IllegalAccessException, IllegalArgumentException, InvocationTargetException, IOException, ScriptException {
		Iterator<TimelinerConfigItem> iter = list.iterator();
		int i = 0;
		while (iter.hasNext()) {
			TimelinerConfigItem tci = iter.next();
			Method method = output.getClass().getMethod(tci.method);
			Object ret = method.invoke(output);
			String value = "";
			if (ret != null) {
				value = tci.format(String.valueOf(ret), se);
			}
			writer.write((i > 0 ? "," : "") + value);
			i++;
		}
		writer.write("\n");
	}

	public static TimelinerConfig ParseConfig(String filename) throws IOException {
		TimelinerConfig tc = new TimelinerConfig();
		tc.list = new ArrayList<TimelinerConfigItem>();
		tc.se = new ScriptEngineManager().getEngineByName("js");

		File file = new File(filename);
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));

		String line;
		while ((line = reader.readLine()) != null) {
			TimelinerConfigItem tci = TimelinerConfigItem.ParseConfigItem(line);
			if (tci != null) {
				tc.list.add(tci);
			}

		}
		reader.close();

		return tc;
	}

	public static void main1(String[] args) throws Exception {
		System.out.println("Hello world!");

		EventOutput output = new EventOutput();
		output.setDriveScene("加速走行");

		TimelinerConfig tc = TimelinerConfig.ParseConfig(args[0]);

		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(args[1]))));

		tc.printHeader(writer);

		for (int i = 0; i < 10; i++) {
			tc.printData(output, writer);
		}

		writer.close();

		System.out.println("Finish!");
	}

	public static void main2(String[] args) throws Exception {
		ScriptEngineManager manager = new ScriptEngineManager();
		ScriptEngine se = manager.getEngineByName("js");
		
		Double val = new Double(1);
		String str = "val = '" + val + "';" + "val /= 100;" + "; '' + val;";
		
		String val1 = "高速道路";
		str = "val = '" + val1 + "';" + "if (val == '高速道路') { val = '高' } else if (val == '一般道路') { val = '般' } else if (val == 'ナビマッチング中') { val = '未' }" + "; '' + val;";
		
		String result =(String) se.eval(str);
		System.out.println(result);
	}
}
