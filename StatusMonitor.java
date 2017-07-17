package com.it_elektronika.luka.tresenje;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import net.wimpi.modbus.Modbus;
import net.wimpi.modbus.ModbusException;
import net.wimpi.modbus.io.ModbusTCPTransaction;
import net.wimpi.modbus.msg.ReadMultipleRegistersRequest;
import net.wimpi.modbus.msg.ReadMultipleRegistersResponse;
import net.wimpi.modbus.msg.WriteMultipleRegistersRequest;
import net.wimpi.modbus.msg.WriteMultipleRegistersResponse;
import net.wimpi.modbus.net.TCPMasterConnection;
import net.wimpi.modbus.procimg.SimpleRegister;

import java.net.InetAddress;
import java.util.ArrayList;


public class StatusMonitor extends AppCompatActivity {

    LinearLayout linearLayout;
    Button buttonRead, buttonWrite, buttonTest;
    ////////////////////////////
    private TextView mregst0;
    private TextView mregst1;
    private TextView mregst2;
    private TextView mregst3;
    private TextView mregst4;
    private TextView mregst5;
    private TextView mregst6;
    private TextView mregst7;
    private TextView mregst8;
    private TextView mregst9;
    private TextView mregst10;
    private TextView mregst11;
    private TextView mregst12;
    private TextView mregst13;
    private TextView mregst14;
    private TextView mregst15;

    ///////////////////////////////

    private TextView gripperStatus;
    private TextView modbusRegs;
    private String resString0;
    private String resString1;
    private String resString2;
    private String resString3;
    private String resString4;
    private String resString5;
    private String resString6;
    private String resString7;
    private String resString8;
    private String resString9;
    private String resString10;
    private String resString11;
    private String resString12;
    private String resString13;
    private String resString14;
    private String resString15;
    private String resString16;
    private String enableString;
    private String lastEnableString;



    private Integer connReg;
    private  ArrayList arrayList;

    //

    private TCPMasterConnection con = null;
    private ModbusTCPTransaction trans = null;

    private InetAddress addr = null;
    private final int port = Modbus.DEFAULT_PORT;
    private boolean quitTask = false;
    private boolean noConn = false;

    Loader loader;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_status_monitor);
        Log.d("STATUS MONITOR", "onCreate");


        ///////////////////////////////////////////////
        mregst0 = (TextView) findViewById(R.id.mrt0);
        mregst1 = (TextView) findViewById(R.id.mrt1);
        mregst2 = (TextView) findViewById(R.id.mrt2);
        mregst3 = (TextView) findViewById(R.id.mrt3);
        mregst4 = (TextView) findViewById(R.id.mrt4);
        mregst5 = (TextView) findViewById(R.id.mrt5);
        mregst6 = (TextView) findViewById(R.id.mrt6);
        mregst7 = (TextView) findViewById(R.id.mrt7);
        mregst8 = (TextView) findViewById(R.id.mrt8);
        mregst9 = (TextView) findViewById(R.id.mrt9);
        mregst10 = (TextView) findViewById(R.id.mrt10);
        mregst11 = (TextView) findViewById(R.id.mrt11);
        mregst12 = (TextView) findViewById(R.id.mrt12);
        mregst13 = (TextView) findViewById(R.id.mrt13);
        mregst14 = (TextView) findViewById(R.id.mrt14);
        mregst15 = (TextView) findViewById(R.id.mrt15);

        ////////////////////////////////////////////////

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        Log.d("STATUS MONITOR", "ON CREATE OPTIONS MENU");
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.optmenu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        Log.d("STATUS MONITOR", "ON OPTIONS ITEM SELECTED");
        int i = item.getItemId();
        Intent intent;
        if (i == R.id.editor)
        {
            intent = new Intent(StatusMonitor.this, EditorLogin.class);
            quitTask = true;
            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }
        else if (i == R.id.loader)
        {
            intent = new Intent(StatusMonitor.this, Loader.class);
            quitTask = true;
            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }
        else
        {
            return super.onOptionsItemSelected(item);
        }
    }

    public void onStop()
    {
        super.onStop();
        Log.d("STATUS MONITOR", "ON STOP");
        quitTask = true;
        try {
            con.close();
        }
        catch (Exception e)
        {

        }
    }
    @Override
    public void onResume() {
        super.onResume();
        Log.d("STATUS MONITOR", "ON RESUME");

        AsyncTask asyncTask = new AsyncTask<Void, Void, Void>() {
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
        Log.d("noConn:", String.valueOf(noConn));
        if (!noConn)
        {
            quitTask = false;
            new AsyncTask<Void, Void, Void>(){

                @Override
                protected Void doInBackground(Void... params)
                {
                    while (!quitTask)
                    {
                        if(con.isConnected())
                        {
                            int startReg = 0;

                            ReadMultipleRegistersRequest req = null; //the request
                            ReadMultipleRegistersResponse res = null; //the response

                            // Prepare the request
                            req = new ReadMultipleRegistersRequest(startReg, 38);

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
                                resString0 = String.valueOf(res.getRegister(0).getValue());
                                resString1 = String.valueOf(res.getRegister(1).getValue());
                                resString2 = String.valueOf(res.getRegister(2).getValue());
                                resString3 = String.valueOf(res.getRegister(3).getValue());
                                resString4 = String.valueOf(res.getRegister(4).getValue());
                                resString5 = String.valueOf(res.getRegister(5).getValue());
                                resString6 = String.valueOf(res.getRegister(6).getValue());
                                resString7 = String.valueOf(res.getRegister(7).getValue());
                                resString8 = String.valueOf(res.getRegister(8).getValue());
                                resString9 = String.valueOf(res.getRegister(9).getValue());
                                resString10 = String.valueOf(res.getRegister(10).getValue());
                                resString11 = String.valueOf(res.getRegister(11).getValue());
                                resString12 = String.valueOf(res.getRegister(12).getValue());
                                resString13 = String.valueOf(res.getRegister(13).getValue());
                                resString14 = String.valueOf(res.getRegister(14).getValue());
                                resString15 = String.valueOf(res.getRegister(15).getValue());
                                resString16 = String.valueOf(res.getRegister(16).getValue());
                                enableString = String.valueOf(res.getRegister(17).getValue());

                                /////////////////////////////////////////////////////////////
                                if (lastEnableString != enableString)
                                {
                                    if (enableString.equals("1"))
                                    {
                                        loader.driveEnable();
                                    }
                                    else
                                    {
                                        loader.driveDisable();
                                    }
                                }
                                lastEnableString = enableString;
                                ////////////////////////////////////////////////////////////
                            }
                            catch (Exception e)
                            {
                                Intent intent_ac = new Intent(StatusMonitor.this, StatusMonitor.class);
                                startActivity(intent_ac);
                                finish();
                                break;
                            }
                            publishProgress();

                        }
                        else
                        {

                        }

                    }
                    return null;
                }

                @Override
                protected void onProgressUpdate(Void... values)
                {
                    // once all points are read & drawn refresh the imageview
                    try {

                        if (resString0.equals("1")) {
                            mregst0.setText("ZAPRTA");
                        } else {
                            mregst0.setText("ODPRTA");
                        }

                        if (resString1.equals("1")) {
                            mregst1.setText("ZAKLENJENA");
                        } else {
                            mregst1.setText("ODKLENJENA");
                        }

                        if (resString2.equals("0")) {
                            mregst2.setText("OK");
                        } else {
                            mregst2.setText("ERROR");
                        }

                        if (resString3.equals("0")) {
                            mregst3.setText("OK");
                        } else {
                            mregst3.setText("ERROR");
                        }

                        if (resString4.equals("1")) {
                            mregst4.setText("BREZ TRESENJA");
                        } else {
                            mregst4.setText("Z TRESENJEM");
                        }

                        if (resString5.equals("1")) {
                            mregst5.setText("OK");
                        } else {
                            mregst5.setText("NOK");
                        }

                        if (resString6.equals("1")) {
                            mregst6.setText("ZAPRTA");
                        } else {
                            mregst6.setText("ODPRTA");
                        }

                        if (resString7.equals("1")) {
                            mregst7.setText("ZAPRT");
                        } else {
                            mregst7.setText("ODPRT");
                        }

                        if (resString8.equals("1")) {
                            mregst8.setText("1");
                        } else {
                            mregst8.setText("0");
                        }

                        if (resString9.equals("1")) {
                            mregst9.setText("1");
                        } else {
                            mregst9.setText("0");
                        }

                        if (resString10.equals("1")) {
                            mregst10.setText("1");
                        } else {
                            mregst10.setText("0");
                        }

                        mregst11.setText(resString11);
                        mregst12.setText(resString12);
                        mregst13.setText(resString13);


                        if (resString14.equals("1")) {
                            mregst14.setText("velika");
                        }
                        else if (resString15.equals("1"))
                        {
                            mregst14.setText("srednja");
                        }
                        else if (resString16.equals("1"))
                        {
                            mregst14.setText("mala");
                        }


                        ///////////////////////////////////////////

                        ///////////////////////////////////////

                    }
                    catch (Exception e)
                    {
                        e.printStackTrace();
                    }
                }

            }.execute();
        }
        else
        {
            Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
        }
    }
}
