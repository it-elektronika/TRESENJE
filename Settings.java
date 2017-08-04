package com.it_elektronika.luka.tresenje;

import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
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




public class Settings extends AppCompatActivity {


    private boolean noConn = false;
    private TCPMasterConnection con = null;
    private ModbusTCPTransaction trans = null;
    private InetAddress addr = null;
    private final int port = Modbus.DEFAULT_PORT;

    private EditText et1;
    private EditText et2;
    private EditText et3;
    private EditText et4;

    private Integer tim1;
    private Integer tim2;
    private Integer tim3;
    private Integer tim4;

    private Integer et1_val;
    private Integer et2_val;
    private Integer et3_val;
    private Integer et4_val;

    private Integer cycleValue;


    private Intent mintent;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        et1 = (EditText) findViewById(R.id.dovoljen_cas_netresenjaET);
        et2 = (EditText) findViewById(R.id.cas_do_zacetka_tresenjaET);
        et3 = (EditText) findViewById(R.id.cas_do_nepolnega_tresenjaET);
        et4 = (EditText) findViewById(R.id.zapiranje_gripperjaET);
        et1_val = 99;
        et2_val = 99;
        et3_val = 99;
        et4_val = 99;
        new AsyncTask<Void, Void, Void>()
        {
            @Override
            protected Void doInBackground(Void... params)
            {
                try
                {
                    addr = InetAddress.getByName("192.168.1.90");
                    con = new TCPMasterConnection(addr);
                    con.setPort(port);
                    con.connect();
                    noConn = false;
                    Log.d("STATUS MONITOR", "CONNECTION ESTABLISHED");
                }
                catch (Exception e)
                {
                    Log.d("STATUS MONITOR", "NO CONNECTION", e);
                    noConn = true;
                    //Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
                }
                return null;
            }
        }.execute();

        et1.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });

        et2.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });

        et3.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });

        et4.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });

        getCycleVal();
        read_data();
        save_data();
    }

    public void onStop()
    {
        super.onStop();

        try
        {
            con.close();
        }
        catch (Exception ignored)
        {
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        try
        {
            Log.d("STATUS MONITOR", "ON CREATE OPTIONS MENU");
            MenuInflater inflater = getMenuInflater();
            inflater.inflate(R.menu.optmenu_ed, menu);
        }
        catch (Exception ignored)
        {

        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        Log.d("STATUS MONITOR", "ON OPTIONS ITEM SELECTED");
        int i = item.getItemId();
        Intent intent;
        if (i == R.id.back)
        {
            intent = new Intent(Settings.this, StatusMonitor.class);

            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }
        else
        {
            return super.onOptionsItemSelected(item);
        }
    }

    private void read_data()
    {
        if (!noConn)
        {
            new AsyncTask<Void, Void, Void>()
            {
                @Override
                protected Void doInBackground(Void... params)
                {
                    if(con.isConnected())
                    {
                        int startReg = 0;

                        ReadMultipleRegistersRequest req; //the request
                        ReadMultipleRegistersResponse res; //the response

                        req = new ReadMultipleRegistersRequest(startReg, 22);

                        trans = new ModbusTCPTransaction(con);
                        trans.setRequest(req);

                        try
                        {
                            trans.execute();
                        }
                        catch (ModbusException e)
                        {
                            e.printStackTrace();
                        }

                        res = (ReadMultipleRegistersResponse) trans.getResponse();
                        try
                        {
                            tim1 = res.getRegister(18).getValue();
                            tim2 = res.getRegister(19).getValue();
                            tim3 = res.getRegister(20).getValue();
                            tim4 = res.getRegister(21).getValue();
                            ////////////////////////////////////////////////////////////
                        }
                        catch (Exception e)
                        {
                            Intent intent_ac = new Intent(Settings.this, Settings.class);
                            startActivity(intent_ac);
                            finish();
                        }
                        publishProgress();
                    }
                    return null;
                }

                @Override
                protected void onProgressUpdate(Void... values)
                {
                    try
                    {
                        et1.setText(String.valueOf(tim1));
                        et2.setText(String.valueOf(tim2));
                        et3.setText(String.valueOf(tim3));
                        et4.setText(String.valueOf(tim4));
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


    private void getTValues()
    {
        try
        {
            et1_val = Integer.parseInt(et1.getText().toString());
            et2_val = Integer.parseInt(et2.getText().toString());
            et3_val = Integer.parseInt(et3.getText().toString());
            et4_val = Integer.parseInt(et4.getText().toString());

        }
        catch(Exception ignored)
        {

        }
    }
    private void getCycleVal() {
        //////////////////////////////////////

        if (!noConn)
        {
            new AsyncTask<Void, Void, Void>()
            {
                @Override
                protected Void doInBackground(Void... params)
                {
                    if (con.isConnected())
                    {
                        int startReg = 0;

                        ReadMultipleRegistersRequest req; //the request
                        ReadMultipleRegistersResponse res; //the response

                        // Prepare the request
                        req = new ReadMultipleRegistersRequest(startReg, 18);

                        // Prepare the transaction
                        trans = new ModbusTCPTransaction(con);
                        trans.setRequest(req);

                        try
                        {
                            trans.execute();
                        } catch (ModbusException e)
                        {
                            e.printStackTrace();
                        }

                        res = (ReadMultipleRegistersResponse) trans.getResponse();
                        try
                        {
                            cycleValue = res.getRegister(17).getValue();


                            ////////////////////////////////////////////////////////////
                        } catch (Exception e)
                        {
                            Intent intent_ac = new Intent(Settings.this, StatusMonitor.class);
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

    }

    private void save_data()
    {
        Button button = (Button) findViewById(R.id.save_timers);

        button.setOnClickListener(new View.OnClickListener()
        {
            public void onClick(View v)
            {
                getTValues();
                getCycleVal();

                try
                {
                    Thread.sleep(2000);
                }
                catch (InterruptedException e)
                {
                    e.printStackTrace();
                }

                if(cycleValue.equals(0))
                {
                    Log.d("SAVE_DATA", "ONCLICK");

                    if (!noConn)
                    {
                        new AsyncTask<Void, Void, Void>()
                        {
                            @Override
                            protected Void doInBackground(Void... params)
                            {
                                if(con.isConnected())
                                {
                                    int startReg = 18;

                                    WriteMultipleRegistersRequest req;
                                    WriteMultipleRegistersResponse res = null;

                                    SimpleRegister[] hr = new SimpleRegister[4];

                                    hr[0]=new SimpleRegister(et1_val);
                                    hr[1]=new SimpleRegister(et2_val);
                                    hr[2]=new SimpleRegister(et3_val);
                                    hr[3]=new SimpleRegister(et4_val);

                                    req = new WriteMultipleRegistersRequest(startReg, hr);

                                    trans = new ModbusTCPTransaction(con);
                                    trans.setRequest(req);

                                    try
                                    {
                                        trans.execute();
                                    }
                                    catch (ModbusException e)
                                    {
                                        e.printStackTrace();
                                    }

                                    res = (WriteMultipleRegistersResponse) trans.getResponse();


                                }
                                return null;
                            }
                        }.execute();
                        Toast.makeText(getApplicationContext(), "SETTINGS SAVED", Toast.LENGTH_LONG).show();
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "NO CONNECTION TO PLC", Toast.LENGTH_LONG).show();
                    }
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "CANNOT CHANGE VALUES WHEN IN CYCLE", Toast.LENGTH_LONG).show();

                    mintent = new Intent(Settings.this, StatusMonitor.class);
                    startActivity(mintent);
                }

            }
        });
    }
}
