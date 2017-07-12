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


public class StatusMonitor extends AppCompatActivity {

    LinearLayout linearLayout;
    Button buttonRead, buttonWrite, buttonTest;
    private ImageView gripperStatus;
    private String resString;
    private Integer connReg;

    private TCPMasterConnection con = null;
    private ModbusTCPTransaction trans = null;

    private InetAddress addr = null;
    private final int port = Modbus.DEFAULT_PORT;
    private boolean quitTask = false;
    private boolean noConn = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_status_monitor);
        Log.d("STATUS MONITOR", "onCreate");

        //testsend = (Button)findViewById(R.id.testsend);

        gripperStatus = (ImageView) findViewById(R.id.imageView);

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
        if (noConn == false)
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
                            req = new ReadMultipleRegistersRequest(startReg, 3);

                            // Prepare the transaction
                            trans = new ModbusTCPTransaction(con);
                            trans.setRequest(req);

                            // execute the transaction

                            try {
                                trans.execute();
                            } catch (ModbusException e) {
                                e.printStackTrace();
                            }


                            //get the response

                            res = (ReadMultipleRegistersResponse) trans.getResponse();
                            try{
                                resString = String.valueOf(res.getRegister(0).getValue());
                            }
                            catch (Exception e){
                                Intent intent_ac = new Intent(StatusMonitor.this, StatusMonitor.class);
                                startActivity(intent_ac);
                                finish();
                                break;

                            }
                            publishProgress();
                            //Log.d("read response:", resString);

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
                    try
                    {
                        Log.d("read response:", resString);

                        if(resString.equals("1"))
                        {
                            Log.d("enka", resString);
                            gripperStatus.setImageResource(android.R.drawable.presence_online);

                        }
                        else {
                            Log.d("nula", resString);
                            gripperStatus.setImageResource(android.R.drawable.presence_busy);
                        }
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
