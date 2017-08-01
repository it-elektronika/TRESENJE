package com.it_elektronika.luka.tresenje;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class EditorLogin extends AppCompatActivity {

    private final String pass = "tkk";
    private EditText editText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_editor_login);
        Log.d("EDITOR LOGIN", "ON CREATE");

        editText = (EditText) findViewById(R.id.editText);
        Button button = (Button) findViewById(R.id.loginbutton);
        button.setOnClickListener(new OnClickListener()
        {
            public void onClick(View v)
            {
                Intent intent = new Intent(EditorLogin.this, Editor.class);

                if(editText.getText().toString().equals(pass))
                {
                    Log.d("EDITOR LOGIN", "LOGIN PRESSED");
                    startActivity(intent);
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "INCORRECT PASSWORD", Toast.LENGTH_LONG).show();
                }
            }
        });


        editText.setOnTouchListener(new View.OnTouchListener()
        {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });
    }
    @Override
    protected void onStop()
    {
        super.onStop();
        Log.d("EDITOR LOGIN", "ON STOP");
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
            intent = new Intent(EditorLogin.this, StatusMonitor.class);
            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }

        else
        {
            return super.onOptionsItemSelected(item);
        }
    }
}
