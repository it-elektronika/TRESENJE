package com.it_elektronika.luka.tresenje;

import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import net.wimpi.modbus.Modbus;
import net.wimpi.modbus.ModbusException;
import net.wimpi.modbus.io.ModbusTCPTransaction;
import net.wimpi.modbus.msg.ReadMultipleRegistersRequest;
import net.wimpi.modbus.msg.ReadMultipleRegistersResponse;
import net.wimpi.modbus.net.TCPMasterConnection;

import java.io.BufferedInputStream;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;


public class Loader extends AppCompatActivity {

    private ListView loaderlistview;
    private ArrayList loaderStr;
    private ArrayAdapter loaderadapter;
    private DataBaseHelper myDb;
    private StatusMonitor statusMonitor;

    String resString;

    private TCPMasterConnection con = null;
    private ModbusTCPTransaction trans = null;

    private InetAddress addr = null;
    private final int port = Modbus.DEFAULT_PORT;
    Button sendButton;

    private String ip_adrs;
    private Socket es;
    private OutputStream os;
    private FilterInputStream is;

    private Integer posCounter;
    private Integer stepCounter;
    private Integer nextMove;
    private Integer a1;
    private Integer a2;
    private Integer vel;
    private Integer acc;
    private Integer dec;
    private Integer dly;
    private Integer exec;
    private Integer transId;
    private Integer i;
    private Integer homePos;
    private Intent mintent;
    private Integer cycleValue;


    private boolean quitTask = false;
    private boolean noConn = false;



    private String selectedFromList;
    Integer j;

    byte obuf[];
    byte ibuf[];

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loader);

        Log.d("LOADER", "onCreate");
        selectedFromList = "";
        myDb = new DataBaseHelper(this);
        transId = 0;
        loaderlistview = (ListView)findViewById(R.id.loaderlistview);
        loaderStr = new ArrayList<>();
        loaderadapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, loaderStr);
        loaderlistview.setAdapter(loaderadapter);
        loaderlistview.setChoiceMode(AbsListView.CHOICE_MODE_SINGLE);
        loaderlistview.setSelector(android.R.color.holo_blue_light);
        statusMonitor = new StatusMonitor();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    addr = InetAddress.getByName("192.168.1.90");

                    con = new TCPMasterConnection(addr);
                    con.setPort(port);
                    con.connect();
                    noConn = false;
                    Log.d("STATUS MONITOR", "CONNECTION ESTABLISHED");
                } catch (Exception e) {
                    Log.d("STATUS MONITOR", "NO CONNECTION", e);
                    noConn = true;
                    //Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
                }
                return null;
            }
        }.execute();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    ip_adrs = "192.168.1.30";
                    es = new Socket(ip_adrs, 502);
                    os = es.getOutputStream();
                    is = new BufferedInputStream(es.getInputStream());
                    Log.d("LOADER", "CONNECTION ESTABLISHED");
                } catch (Exception e) {
                    Log.d("LOADER", "CONNECTION ERROR", e);
                }
                return null;
            }
        }.execute();
        getResString17();
        sendData();
        populateList();
        getSelected();

    }
    @Override
    public void onStop()
    {
        super.onStop();
        Log.d("LOADER", "onStop");
        try
        {
            es.close();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
    @Override
    public void onResume()
    {
        super.onResume();
        Log.d("LOADER", "onResume");
    }




    public void driveEnable(){
        Log.d("LOADER", "DRIVE ENABLE");

        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                byte obuf[] = new byte[261];
                byte ibuf[] = new byte[261];

                obuf[0] = (byte) (7 >> 8);
                obuf[1] = (byte) (7 & 0xff);
                obuf[2] = (byte) (0);
                obuf[3] = (byte) (0);
                obuf[4] = (byte) (11 >> 8);
                obuf[5] = (byte) (11 & 0xff);
                obuf[6] = (byte) (1);
                obuf[7] = (byte) (16);
                obuf[8] = (byte) (254 >> 8);
                obuf[9] = (byte) (254 & 0xff);
                obuf[10] = (byte) (2 >> 8);
                obuf[11] = (byte) (2 & 0xff);
                obuf[12] = (byte) (4);
                obuf[13] = (byte) (0);
                obuf[14] = (byte) (0);
                obuf[15] = (byte) (0);
                obuf[16] = (byte) ((1)&0xff);

                try
                {
                    os.write(obuf, 0, 17);
                    Log.d("DRIVE ENABLE:", "MESSAGE SENT");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }

                // read response
                try
                {
                    is.read(ibuf, 0, 261);
                    Log.d("DRIVE ENABLE:", "MESSAGE RECEIVED");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                return null;
            }
        }.execute();
    }

    public void driveDisable(){
        Log.d("LOADER", "DRIVE ENABLE");

        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                byte obuf[] = new byte[261];
                byte ibuf[] = new byte[261];

                obuf[0] = (byte) (7 >> 8);
                obuf[1] = (byte) (7 & 0xff);
                obuf[2] = (byte) (0);
                obuf[3] = (byte) (0);
                obuf[4] = (byte) (11 >> 8);
                obuf[5] = (byte) (11 & 0xff);
                obuf[6] = (byte) (1);
                obuf[7] = (byte) (16);
                obuf[8] = (byte) (254 >> 8);
                obuf[9] = (byte) (254 & 0xff);
                obuf[10] = (byte) (2 >> 8);
                obuf[11] = (byte) (2 & 0xff);
                obuf[12] = (byte) (4);
                obuf[13] = (byte) (0);
                obuf[14] = (byte) (0);
                obuf[15] = (byte) (0);
                obuf[16] = (byte) ((0)&0xff);

                try
                {
                    os.write(obuf, 0, 17);
                    Log.d("DRIVE ENABLE:", "MESSAGE SENT");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }

                // read response
                try
                {
                    is.read(ibuf, 0, 261);
                    Log.d("DRIVE ENABLE:", "MESSAGE RECEIVED");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                return null;
            }
        }.execute();
    }

    public void startMotion(){
        Log.d("LOADER", "START MOTION");

        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                byte obuf[] = new byte[261];
                byte ibuf[] = new byte[261];

                obuf[0] = (byte) (transId >> 8);
                obuf[1] = (byte) (transId & 0xff);
                obuf[2] = (byte) (0);
                obuf[3] = (byte) (0);
                obuf[4] = (byte) (15 >> 8);
                obuf[5] = (byte) (15 & 0xff);
                obuf[6] = (byte) (1);
                obuf[7] = (byte) (16);
                obuf[8] = (byte) (8212 >> 8);
                obuf[9] = (byte) (8212 & 0xff);
                obuf[10] = (byte) (4 >> 8);
                obuf[11] = (byte) (4 & 0xff);
                obuf[12] = (byte) (8);
                obuf[13] = (byte) (0);
                obuf[14] = (byte) (0);
                obuf[15] = (byte) (0);
                obuf[16] = (byte) ((1)&0xff);
                obuf[17] = (byte) (0);
                obuf[18] = (byte) (0);
                obuf[19] = (byte) (0);
                obuf[20] = (byte) (0);

                try
                {
                    os.write(obuf, 0, 21);
                    Log.d("START MOTION:", "MESSAGE SENT");
                    transId++;
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                try
                {
                    is.read(ibuf, 0, 261);
                    Log.d("START MOTION:", "MESSAGE RECEIVED");
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                return null;
            }
        }.execute();
    }

    private void clearTable()
    {
        Log.d("LOADER", "CLEAR TABLE");
        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                posCounter = 0;
                for(i = 0; i < 128; ++i)
                {
                    byte obuf[] = new byte[261];
                    byte ibuf[] = new byte[261];

                    obuf[0] = (byte) (transId >> 8);
                    obuf[1] = (byte) (transId & 0xff);
                    obuf[2] = (byte) (0);
                    obuf[3] = (byte) (0);
                    obuf[4] = (byte) (11 >> 8);
                    obuf[5] = (byte) (11 & 0xff);
                    obuf[6] = (byte) (1);
                    obuf[7] = (byte) (16);
                    obuf[8] = (byte) (530 >> 8);
                    obuf[9] = (byte) (530 & 0xff);
                    obuf[10] = (byte) (2 >> 8);
                    obuf[11] = (byte) (2 & 0xff);
                    obuf[12] = (byte) (4);
                    obuf[13] = (byte) ((posCounter >> 24)&0xff);
                    obuf[14] = (byte) ((posCounter >> 16)&0xff);
                    obuf[15] = (byte) ((posCounter >> 8)&0xff);
                    obuf[16] = (byte) ((posCounter)&0xff);

                    try
                    {
                        os.write(obuf, 0, 17);
                        Log.d("CLEAR TABLE:", "MESSAGE SENT");
                        transId++;
                        posCounter++;
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }

                    try
                    {
                        is.read(ibuf, 0, 261);
                        Log.d("CLEAR TABLE:", "MESSAGE RECEIVED");
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }
                }
                return null;
            }
        }.execute();
    }

    private void homePosition()
    {
        Log.d("LOADER", "HOME POSITION");
        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                byte obuf[] = new byte[261];
                byte ibuf[] = new byte[261];

                homePos = 0;

                obuf[0] = (byte) (transId >> 8);
                obuf[1] = (byte) (transId & 0xff);
                obuf[2] = (byte) (0);
                obuf[3] = (byte) (0);
                obuf[4] = (byte) (47 >> 8);
                obuf[5] = (byte) (47 & 0xff);
                obuf[6] = (byte) (1);
                obuf[7] = (byte) (16);
                obuf[8] = (byte) (8192 >> 8);
                obuf[9] = (byte) (8192 & 0xff);
                obuf[10] = (byte) (20 >> 8);
                obuf[11] = (byte) (20 & 0xff);
                obuf[12] = (byte) (40);
                obuf[13] = (byte) ((stepCounter >> 24)&0xff);
                obuf[14] = (byte) ((stepCounter >> 16)&0xff);
                obuf[15] = (byte) ((stepCounter >> 8)&0xff);
                obuf[16] = (byte) ((stepCounter)&0xff);
                obuf[17] = (byte) (0);
                obuf[18] = (byte) (0);
                obuf[19] = (byte) ((homePos >> 8)&0xff); // position
                obuf[20] = (byte) ((homePos) & 0xff);
                obuf[21] = (byte) (0);
                obuf[22] = (byte) ((300000 >> 16)&0xff);
                obuf[23] = (byte) ((300000 >> 8)&0xff);
                obuf[24] = (byte) ((300000)&0xff);
                obuf[25] = (byte) (0);
                obuf[26] = (byte) (0);
                obuf[27] = (byte) (0);
                obuf[28] = (byte) ((16)&0xff);
                obuf[29] = (byte) (0);
                obuf[30] = (byte) ((100000 >> 16)&0xff);
                obuf[31] = (byte) ((100000 >> 8)&0xff);
                obuf[32] = (byte) ((100000)&0xff);
                obuf[33] = (byte) (0);
                obuf[34] = (byte) ((100000 >> 16)&0xff);
                obuf[35] = (byte) ((100000 >> 8)&0xff);
                obuf[36] = (byte) ((100000)&0xff);
                obuf[37] = (byte) ((nextMove >> 24)&0xff);
                obuf[38] = (byte) ((nextMove >> 16)&0xff);
                obuf[39] = (byte) ((nextMove >> 8)&0xff);
                obuf[40] = (byte) ((nextMove)&0xff);
                obuf[41] = (byte) (0);
                obuf[42] = (byte) (0);
                obuf[43] = (byte) (0);
                obuf[44] = (byte) (0);
                obuf[45] = (byte) (0);
                obuf[46] = (byte) (0);
                obuf[47] = (byte) (0);
                obuf[48] = (byte) ((1)&0xff);
                obuf[49] = (byte) (0);
                obuf[50] = (byte) (0);
                obuf[51] = (byte) (0);
                obuf[52] = (byte) ((1)&0xff);

                try
                {
                    os.write(obuf, 0, 53);
                    Log.d("HOME POSITION:", "MESSAGE SENT");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }

                // read response
                try
                {
                    is.read(ibuf, 0, 261);
                    Log.d("HOME POSITION:", "MESSAGE RECEIVED");

                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                return null;
            }
        }.execute();
    }

    private void sendData()
    {


        final Button sendButton = (Button) findViewById(R.id.sendbutton);
        sendButton.setOnClickListener(new View.OnClickListener()
        {

            public void onClick(View v)
            {



                Log.d("SEND DATA", "onCLick");
                if(selectedFromList.isEmpty())
                {
                    Toast.makeText(getApplicationContext(), "CHOOSE RECIPE TO SEND", Toast.LENGTH_LONG).show();
                }

                else
                {
                    getResString17();

                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }


                    Log.d("before if test:", cycleValue.toString());
                    if(cycleValue == 0)
                    {

                        i = 0;
                        stepCounter = 0;
                        nextMove = 1;
                        clearTable();

                        new AsyncTask<Void, Void, Void>() {
                            @Override
                            protected Void doInBackground(Void... params) {
                                for (i = 0; i < myDb.yData.length; i = i + 7) {
                                    Log.d("SEND DATA", "doInBackground");
                                    a1 = myDb.yData[i] * (-29000);
                                    a2 = myDb.yData[i + 1] * (-29000);
                                    vel = myDb.yData[i + 2] * 30000;
                                    acc = myDb.yData[i + 3] * 1000;
                                    dec = myDb.yData[i + 4] * 1000;
                                    dly = myDb.yData[i + 5];
                                    exec = myDb.yData[i + 6];

                                    if (exec > 0) {
                                        for (int j = 0; j < exec; ++j) {
                                            byte obuf[] = new byte[261];
                                            byte ibuf[] = new byte[261];

                                            Log.d("I:", String.valueOf(i));
                                            //////////////////////////////
                                            ////////// A1 ////////////////
                                            obuf[0] = (byte) (transId >> 8);
                                            obuf[1] = (byte) (transId & 0xff);
                                            obuf[2] = (byte) (0);
                                            obuf[3] = (byte) (0);
                                            obuf[4] = (byte) (47 >> 8);
                                            obuf[5] = (byte) (47 & 0xff);
                                            obuf[6] = (byte) (1);
                                            obuf[7] = (byte) (16);
                                            obuf[8] = (byte) (8192 >> 8);
                                            obuf[9] = (byte) (8192 & 0xff);
                                            obuf[10] = (byte) (20 >> 8);
                                            obuf[11] = (byte) (20 & 0xff);
                                            obuf[12] = (byte) (40);
                                            obuf[13] = (byte) ((stepCounter >> 24) & 0xff);
                                            obuf[14] = (byte) ((stepCounter >> 16) & 0xff);
                                            obuf[15] = (byte) ((stepCounter >> 8) & 0xff);
                                            obuf[16] = (byte) ((stepCounter) & 0xff);
                                            obuf[17] = (byte) ((a1 >> 24) & 0xff);
                                            obuf[18] = (byte) ((a1 >> 16) & 0xff);
                                            obuf[19] = (byte) ((a1 >> 8) & 0xff);
                                            obuf[20] = (byte) ((a1) & 0xff);
                                            obuf[21] = (byte) ((vel >> 24) & 0xff);
                                            obuf[22] = (byte) ((vel >> 16) & 0xff);
                                            obuf[23] = (byte) ((vel >> 8) & 0xff);
                                            obuf[24] = (byte) ((vel) & 0xff);
                                            if (dly.equals(0)) {
                                                obuf[25] = (byte) (0);
                                                obuf[26] = (byte) (0);
                                                obuf[27] = (byte) (0);
                                                obuf[28] = (byte) ((16) & 0xff);
                                            } else {
                                                obuf[25] = (byte) (0);
                                                obuf[26] = (byte) (0);
                                                obuf[27] = (byte) (0);
                                                obuf[28] = (byte) ((48) & 0xff);
                                            }
                                            obuf[29] = (byte) ((acc >> 24) & 0xff);
                                            obuf[30] = (byte) ((acc >> 16) & 0xff);
                                            obuf[31] = (byte) ((acc >> 8) & 0xff);
                                            obuf[32] = (byte) ((acc) & 0xff);
                                            obuf[33] = (byte) ((dec >> 24) & 0xff);
                                            obuf[34] = (byte) ((dec >> 16) & 0xff);
                                            obuf[35] = (byte) ((dec >> 8) & 0xff);
                                            obuf[36] = (byte) ((dec) & 0xff);
                                            obuf[37] = (byte) ((nextMove >> 24) & 0xff);
                                            obuf[38] = (byte) ((nextMove >> 16) & 0xff);
                                            obuf[39] = (byte) ((nextMove >> 8) & 0xff);
                                            obuf[40] = (byte) ((nextMove) & 0xff);
                                            obuf[41] = (byte) ((dly >> 24) & 0xff);
                                            obuf[42] = (byte) ((dly >> 16) & 0xff);
                                            obuf[43] = (byte) ((dly >> 8) & 0xff);
                                            obuf[44] = (byte) ((dly) & 0xff);
                                            obuf[45] = (byte) (0);
                                            obuf[46] = (byte) (0);
                                            obuf[47] = (byte) (0);
                                            obuf[48] = (byte) ((1) & 0xff);
                                            obuf[49] = (byte) (0);
                                            obuf[50] = (byte) (0);
                                            obuf[51] = (byte) (0);
                                            obuf[52] = (byte) ((1) & 0xff);

                                            try {
                                                os.write(obuf, 0, 53);
                                                Log.d("SEND DATA A1:", "MESSAGE SENT");
                                                transId++;
                                                stepCounter++;
                                                nextMove++;
                                            } catch (IOException e) {
                                                e.printStackTrace();
                                            }
                                            try {
                                                is.read(ibuf, 0, 261);
                                                Log.d("SEND DATA A1:", "MESSAGE RECEIVED");
                                            } catch (IOException e) {
                                                e.printStackTrace();
                                            }
                                            //////////////////////////////
                                            ////////// A2 ////////////////
                                            obuf[0] = (byte) (transId >> 8);
                                            obuf[1] = (byte) (transId & 0xff);
                                            obuf[2] = (byte) (0);
                                            obuf[3] = (byte) (0);
                                            obuf[4] = (byte) (47 >> 8);
                                            obuf[5] = (byte) (47 & 0xff);
                                            obuf[6] = (byte) (1);
                                            obuf[7] = (byte) (16);
                                            obuf[8] = (byte) (8192 >> 8);
                                            obuf[9] = (byte) (8192 & 0xff);
                                            obuf[10] = (byte) (20 >> 8);
                                            obuf[11] = (byte) (20 & 0xff);
                                            obuf[12] = (byte) (40);
                                            obuf[13] = (byte) ((stepCounter >> 24) & 0xff);
                                            obuf[14] = (byte) ((stepCounter >> 16) & 0xff);
                                            obuf[15] = (byte) ((stepCounter >> 8) & 0xff);
                                            obuf[16] = (byte) ((stepCounter) & 0xff);
                                            obuf[17] = (byte) ((a2 >> 24) & 0xff);
                                            obuf[18] = (byte) ((a2 >> 16) & 0xff);
                                            obuf[19] = (byte) ((a2 >> 8) & 0xff);
                                            obuf[20] = (byte) ((a2) & 0xff);
                                            obuf[21] = (byte) ((vel >> 24) & 0xff);
                                            obuf[22] = (byte) ((vel >> 16) & 0xff);
                                            obuf[23] = (byte) ((vel >> 8) & 0xff);
                                            obuf[24] = (byte) ((vel) & 0xff);
                                            if (dly.equals(0)) {
                                                obuf[25] = (byte) (0);
                                                obuf[26] = (byte) (0);
                                                obuf[27] = (byte) (0);
                                                obuf[28] = (byte) ((16) & 0xff);
                                            } else {
                                                obuf[25] = (byte) (0);
                                                obuf[26] = (byte) (0);
                                                obuf[27] = (byte) (0);
                                                obuf[28] = (byte) ((48) & 0xff);
                                            }
                                            obuf[29] = (byte) ((acc >> 24) & 0xff);
                                            obuf[30] = (byte) ((acc >> 16) & 0xff);
                                            obuf[31] = (byte) ((acc >> 8) & 0xff);
                                            obuf[32] = (byte) ((acc) & 0xff);
                                            obuf[33] = (byte) ((dec >> 24) & 0xff);
                                            obuf[34] = (byte) ((dec >> 16) & 0xff);
                                            obuf[35] = (byte) ((dec >> 8) & 0xff);
                                            obuf[36] = (byte) ((dec) & 0xff);
                                            obuf[37] = (byte) ((nextMove >> 24) & 0xff);
                                            obuf[38] = (byte) ((nextMove >> 16) & 0xff);
                                            obuf[39] = (byte) ((nextMove >> 8) & 0xff);
                                            obuf[40] = (byte) ((nextMove) & 0xff);
                                            obuf[41] = (byte) ((dly >> 24) & 0xff);
                                            obuf[42] = (byte) ((dly >> 16) & 0xff);
                                            obuf[43] = (byte) ((dly >> 8) & 0xff);
                                            obuf[44] = (byte) ((dly) & 0xff);
                                            obuf[45] = (byte) (0);
                                            obuf[46] = (byte) (0);
                                            obuf[47] = (byte) (0);
                                            obuf[48] = (byte) ((1) & 0xff);
                                            obuf[49] = (byte) (0);
                                            obuf[50] = (byte) (0);
                                            obuf[51] = (byte) (0);
                                            obuf[52] = (byte) ((1) & 0xff);

                                            try {
                                                os.write(obuf, 0, 53);
                                                Log.d("SEND DATA A2:", "MESSAGE SENT");
                                                transId++;
                                                stepCounter++;
                                                nextMove++;
                                            } catch (IOException e) {
                                                e.printStackTrace();
                                            }

                                            // read response
                                            try {
                                                is.read(ibuf, 0, 261);
                                                Log.d("SEND DATA A2", "MESSAGE RECEIVED");
                                            } catch (IOException e) {
                                                e.printStackTrace();
                                            }
                                        }
                                    }
                                }
                                homePosition();
                                publishProgress();
                                return null;
                            }

                            @Override
                            protected void onProgressUpdate(Void... values) {
                                Toast.makeText(getApplicationContext(), "SENDING FINISHED", Toast.LENGTH_LONG).show();
                            }
                        }.execute();
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "CANNOT SEND RECIPE WHEN IN CYCLE", Toast.LENGTH_LONG).show();
                        Log.d("else:", cycleValue.toString());
                        mintent = new Intent(Loader.this, StatusMonitor.class);
                        startActivity(mintent);

                    }
                }
            }
        });
    }


    private void populateList()
    {
        Log.d("LOADER", "POPULATE LIST");
        loaderStr.removeAll(loaderStr);
        myDb.getAllTables(loaderStr,loaderadapter);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        Log.d("LOADER", "ON CREATE OPTIONS MENU");
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.optmenu_load, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        Log.d("LOADER", "ON OPTION ITEM SELECTED");
        int i = item.getItemId();
        Intent intent;
        if (i == R.id.back)
        {
            Log.v("Intent:", item.toString());
            intent = new Intent(Loader.this, StatusMonitor.class);
            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }
        else
        {
            return super.onOptionsItemSelected(item);
        }
    }

    private void getSelected()
    {
        Log.d("LOADER", "GET SELECTED");
        loaderlistview.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                selectedFromList = (loaderlistview.getItemAtPosition(position).toString());

                //////////////////////////////////////////
                myDb.getTN(selectedFromList);

                myDb.getAllData();
                myDb.tableCheck();
                //////////////////////////////////////////
                loaderlistview.setSelector(android.R.color.holo_blue_light);
                Log.d("selected:", selectedFromList);
            }
        });
    }



    private void getResString17()
    {

        //////////////////////////////////////
        Log.d("noConn:", String.valueOf(noConn));
        if (!noConn)
        {

            new AsyncTask<Void, Void, Void>(){

                @Override
                protected Void doInBackground(Void... params)
                {

                    if(con.isConnected())
                    {
                        int startReg = 0;

                        ReadMultipleRegistersRequest req; //the request
                        ReadMultipleRegistersResponse res; //the response

                        // Prepare the request
                        req = new ReadMultipleRegistersRequest(startReg, 18);

                        // Prepare the transaction
                        trans = new ModbusTCPTransaction(con);
                        trans.setRequest(req);

                        // execute the transaction

                        try
                        {
                            trans.execute();
                        }
                        catch (ModbusException e)
                        {
                            e.printStackTrace();
                        }


                        //get the response

                        res = (ReadMultipleRegistersResponse) trans.getResponse();
                        try
                        {

                            cycleValue = res.getRegister(17).getValue();
                            Log.d("actual:", cycleValue.toString());
                            ////////////////////////////////////////////////////////////
                        }
                        catch (Exception e)
                        {
                            Intent intent_ac = new Intent(Loader.this, StatusMonitor.class);
                            startActivity(intent_ac);
                            finish();

                        }
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
                    }

                    return null;
                }
            }.execute();
        }
        else
        {
            Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
        }
        //return cycleValue;


    }



    public void makeConn(){
        try {
            ip_adrs = "192.168.1.30";
            es = new Socket(ip_adrs, 502);
            os = es.getOutputStream();
            is = new BufferedInputStream(es.getInputStream());
            Log.d("LOADER", "CONNECTION ESTABLISHED");
        } catch (Exception e) {
            Log.d("LOADER", "CONNECTION ERROR", e);
        }
    }

}










