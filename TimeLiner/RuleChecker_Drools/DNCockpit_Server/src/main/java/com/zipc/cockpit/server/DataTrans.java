package com.zipc.cockpit.server;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;

public class DataTrans {
	
	private int HEADER_SIZE = 5;
	private int ACK_SIZE = 3;
	
	// 通信先タスクのIPとPort
	private Socket socket = null;
	private String ip;
	private int portNo;

	// 自サーバー側
	private ServerSocket serverSocket = null;
	private String myServerIp;
	private int myServerPortNo;
	
	// AppAckヘッダ
	byte[] appAckHeader = new byte[HEADER_SIZE];
	byte[] appAckData = new byte[HEADER_SIZE + ACK_SIZE];
	
	// コンストラクタ(通信先情報か、自サーバ情報を引数にとる)
	public DataTrans(String _ip, int _portNo) {
		ip = new String(_ip);
		portNo   = _portNo;

		myServerIp = new String(_ip);
		myServerPortNo = _portNo;
		
		appAckHeader[0] = '@';
		appAckHeader[1] = 0x00;
		appAckHeader[2] = 0x00;
		appAckHeader[3] = 0x00;
		appAckHeader[4] = 0x03;

		System.arraycopy(appAckHeader, 0, appAckData, 0, appAckHeader.length);
	}

	public boolean connect() {
		while(socket == null) {
			try {
				socket = new Socket(ip, portNo);
			} catch (UnknownHostException e) {
				e.printStackTrace();
				return false;
			} catch (IOException e) {
				return false;
			}
		}
		return true;
	}

	public void disconnect() {
		if(socket != null) {
			try {
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			socket = null;
		}
	}

	public boolean send(String _sendXml) {
		byte[] sendXml = _sendXml.getBytes();
		byte[] sendData = new byte[HEADER_SIZE + sendXml.length];
		sendData[0] = '@';
		// サイズ(ビッグエンディアン)
		sendData[1] = (byte) ((_sendXml.length()&0xFF000000) >> 24);
		sendData[2] = (byte) ((_sendXml.length()&0x00FF0000) >> 16);
		sendData[3] = (byte) ((_sendXml.length()&0x0000FF00) >> 8);
		sendData[4] = (byte)  (_sendXml.length()&0x000000FF);
		System.arraycopy(sendXml, 0, sendData, HEADER_SIZE, sendXml.length);

		// 他タスクへの通信
		OutputStream osm;
		try {
			osm = socket.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		try {
			osm.write(sendData, 0, sendData.length);
		} catch(IOException e) {
			e.printStackTrace();
			return false;
		}

		// AppAck受信
		if(recvAppAck() == false) {
			return false;
		}
		return true;
	}

	public String recv() {
		InputStream ism;
		String xml = null;
		try {
			ism = socket.getInputStream();
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}

		// ヘッダ受信
		byte[] header = new byte[HEADER_SIZE];
		int transSize;
		try {
			transSize = ism.read(header, 0, HEADER_SIZE);
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}

		// 受信失敗なら終了
		if(transSize == -1) {
			return null;
		}

		// ヘッダ読込ができなければ異常終了
		if(transSize != HEADER_SIZE) {
			System.out.println("受信データのヘッダサイズ異常[" + transSize + "]");
			return null;
		}

		// AppAckなら異常終了
		if(Arrays.equals(header, appAckHeader) == true) {
			System.out.println("受信データのヘッダ異常[" + appAckHeader + "]　AppAckが来ている");
			return null;
		}
		
		// 受信データサイズ取得(ビッグエンディアン)
		int dataSize = 0;
		dataSize += (header[1] & 0xFF) << 24;
		dataSize += (header[2] & 0xFF) << 16;
		dataSize += (header[3] & 0xFF) << 8;
		dataSize += (header[4] & 0xFF);

		// 受信データサイズがある場合は受信
		if(dataSize > 0) {
			byte[] data = new byte[dataSize];
			try {
				transSize = ism.read(data, 0, dataSize);
			} catch (IOException e) {
				e.printStackTrace();
				return null;
			}
			try {
				xml = new String(data, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
				return null;
			}
			if(transSize != dataSize) {
				System.out.println("受信データサイズ異常　ヘッダ上サイズ[" + dataSize + "]　readできたサイズ[" + transSize + "]");
				return null;
			}
		}
		// AppAck送信
		if( sendAppAck() == false) {
			return null;
		}
		return xml;
	}

	private boolean sendAppAck() {
		OutputStream osm;
		try {
			osm = socket.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		try {
			osm.write(appAckData, 0, appAckData.length);
		} catch(IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	private boolean recvAppAck() {
		InputStream ism;
		try {
			ism = socket.getInputStream();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}

		// ヘッダ受信
		byte[] header = new byte[HEADER_SIZE];
		byte[] data = new byte[ACK_SIZE];
		int transSize;
		try {
			transSize = ism.read(header, 0, HEADER_SIZE);
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	
		// ヘッダ読込ができなければ異常終了
		if(transSize != HEADER_SIZE) {
			return false;
		}
		
		// AppAck以外なら異常終了
		if(Arrays.equals(header, appAckHeader) == false) {
			return false;
		}

		try {
			transSize = ism.read(data, 0, ACK_SIZE);
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}

		// データ読込ができなければ異常終了
		if(transSize != ACK_SIZE) {
			return false;
		}
		
		return true;
	}

	public boolean startup() {		
		int backLog = 0;

		// InetAddress取得
		InetAddress address;
		try {
			address = InetAddress.getByName(myServerIp);
		} catch (UnknownHostException e) {
			e.printStackTrace();
			return false;
		}

		// サーバーソケット生成
		try {
			serverSocket = new ServerSocket(myServerPortNo, backLog, address);
		} catch (IOException e1) {
			e1.printStackTrace();
			return false;
		}
		return true;
	}

	public boolean accept() {
		try {
			socket = serverSocket.accept();
		} catch (IOException e) {
			return false;
		}
		return true;
	}

	public void shutdown() {
		try {
			if(socket != null) {
				socket.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		socket = null;
		try {
			if(serverSocket != null) {
				serverSocket.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		serverSocket = null;
	}
}