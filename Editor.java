package com.it_elektronika.luka.tresenje;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Color;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.InputFilter;
import android.text.InputType;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

public class Editor extends AppCompatActivity {

    private Button save;
    private Button delete;
    private Button newrec;
    private DataBaseHelper myDb;

    private static Context context;

    private ListView listview;
    private ArrayList<String> lStr;
    private EditText recName;
    private ArrayAdapter<String> adapter;
    private ArrayList<EditText> allET;
    private String tableName;
    private Boolean cond1;
    private Boolean cond2;
    private Boolean cond3;
    private Boolean cond4;
    private Boolean cond5;
    private Boolean cond6;
    private Boolean cond7;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("EDITOR", "ON CREATE");
        Editor.context = getApplicationContext();

        ////////////////////////////////////////////////////////////////////
        LinearLayout topbar = new LinearLayout(this);
        LinearLayout.LayoutParams bottomParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        topbar.setOrientation(LinearLayout.HORIZONTAL);
        topbar.setLayoutParams(bottomParams);

        LinearLayout leftBar = new LinearLayout(this);
        LinearLayout.LayoutParams leftParams = new LinearLayout.LayoutParams(500, LinearLayout.LayoutParams.WRAP_CONTENT);
        leftBar.setOrientation(LinearLayout.VERTICAL);
        leftBar.setLayoutParams(leftParams);

        LinearLayout leftBarSup = new LinearLayout(this);
        LinearLayout.LayoutParams leftsupParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        leftBarSup.setOrientation(LinearLayout.HORIZONTAL);
        leftBarSup.setLayoutParams(leftsupParams);

        LinearLayout leftBarTop = new LinearLayout(this);
        LinearLayout.LayoutParams lefttopParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        leftBarTop.setOrientation(LinearLayout.HORIZONTAL);
        leftBarTop.setLayoutParams(lefttopParams);

        LinearLayout leftBarBot = new LinearLayout(this);
        LinearLayout.LayoutParams leftbotParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        leftBarBot.setOrientation(LinearLayout.HORIZONTAL);
        leftBarBot.setLayoutParams(leftbotParams);

        LinearLayout rightBar = new LinearLayout(this);
        LinearLayout.LayoutParams rightParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.MATCH_PARENT);
        rightBar.setOrientation(LinearLayout.VERTICAL);
        rightBar.setLayoutParams(rightParams);

        myDb = new DataBaseHelper(this);

        listview = new ListView(this);
        lStr = new ArrayList<>();
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, lStr);
        listview.setAdapter(adapter);
        listview.setLayoutParams(rightParams);
        listview.setChoiceMode(AbsListView.CHOICE_MODE_SINGLE);
        listview.setSelector(android.R.color.holo_blue_light);
        rightBar.addView(listview);
        listview.requestFocus();

        TextView recLabel;
        recLabel = new TextView(this);
        recLabel.setText(R.string.name);
        recLabel.setTextSize(25);
        recLabel.setPadding(10,0,10,0);

        leftBarSup.addView(recLabel);

        recName = new EditText(this);
        recName.setTextSize(25);
        recName.setSingleLine();
        recName.setImeOptions(EditorInfo.IME_ACTION_DONE);
        recName.setInputType(InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS);
        recName.setLayoutParams(leftsupParams);
        recName.setFocusable(false);
        recName.setFocusableInTouchMode(false);

        leftBarSup.addView(recName);

        newrec = new Button(this);
        newrec.setText(R.string.neww);
        newrec.setTextSize(25);
        newrec.setWidth(250);

        leftBarTop.addView(newrec);

        save = new Button(this);

        save.setWidth(250);
        save.setText(R.string.save);
        save.setTextSize(25);
        leftBarTop.addView(save);

        delete = new Button(this);
        delete.setLayoutParams(leftbotParams);
        delete.setText(R.string.delete);
        delete.setTextSize(25);
        leftBarBot.addView(delete);

        leftBar.addView(leftBarSup);
        leftBar.addView(leftBarTop);
        leftBar.addView(leftBarBot);

        topbar.addView(leftBar);
        topbar.addView(rightBar);
        //////////////////////////////////////////////////////////

        int number_of_columns = 7;
        GridLayout grid = new GridLayout(this);
        grid.setOrientation(GridLayout.HORIZONTAL);
        grid.setColumnCount(number_of_columns);

        allET = new ArrayList<>();
        for (int i = 0; i < 70; ++i)
        {
            EditText editText = new EditText(this);
            allET.add(editText);
            editText.setText(String.valueOf(0));
            editText.setTextSize(35);
            editText.setPadding(35, 35, 35, 35);
            editText.setEms(4);
            editText.setTextColor(Color.BLACK);
            editText.setInputType(InputType.TYPE_CLASS_NUMBER);
            editText.setImeOptions(EditorInfo.IME_ACTION_DONE);
            InputFilter[] filters = new InputFilter[1];
            filters[0] = new InputFilter.LengthFilter(5);
            editText.setFilters(filters);
            //editText.generateViewId();
            grid.addView(editText);
        }
        ///////////////////////////// RANGE LIMITATION /////////////////////////////////////////////////////
        for (int i = 0; i < allET.size(); i = i + 7)
        {

            InputFilter[] filters = new InputFilter[1];
            filters[0] = new InputFilter.LengthFilter(3);
            allET.get(i).setFilters(filters);
        }
        for (int i = 1; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            InputFilter[] filters = new InputFilter[1];
            filters[0] = new InputFilter.LengthFilter(3);
            allET.get(i).setFilters(filters);
        }
        for (int i = 2; i < allET.size(); i = i + 7)
        {
            InputFilter[] filters = new InputFilter[1];
            filters[0] = new InputFilter.LengthFilter(3);
            allET.get(i).setFilters(filters);
        }
        for (int i = 3; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            allET.get(i).setEms(5);
        }
        for (int i = 4; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            allET.get(i).setEms(5);
        }
        for (int i = 6; i < allET.size(); i = i + 7)
        {

            allET.get(i).setEms(3);
        }
        //////////////////////////////////////////////
        LinearLayout stepCol = new LinearLayout(this);
        LinearLayout.LayoutParams stepColParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        stepCol.setLayoutParams(stepColParams);
        stepCol.setOrientation(LinearLayout.VERTICAL);

        TextView textView;
        for (int i = 1; i < 11; ++i)
        {
            textView = new TextView(this);
            textView.setText(String.valueOf(i));
            textView.setTextSize(35);
            textView.setPadding(35, 35, 35, 35);
            stepCol.addView(textView);
        }
        ///////////////////////////////////////////////////////////////////////////////7
        LinearLayout cont = new LinearLayout(this);
        LinearLayout.LayoutParams contParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        cont.setLayoutParams(contParams);
        cont.setOrientation(LinearLayout.HORIZONTAL);
        cont.addView(stepCol);
        cont.addView(grid);
        ////////////////////////////////////////////////
        ScrollView scrollView = new ScrollView(this);
        ScrollView.LayoutParams scrollParams = new ScrollView.LayoutParams(ScrollView.LayoutParams.MATCH_PARENT, ScrollView.LayoutParams.WRAP_CONTENT);

        scrollView.setLayoutParams(scrollParams);
        scrollView.addView(cont);
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        LinearLayout varRow = new LinearLayout(this);
        LinearLayout.LayoutParams varRowParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        varRow.setLayoutParams(varRowParams);

        varRow.setOrientation(LinearLayout.HORIZONTAL);

        String[] spremenljivkeTekst = {"#", "ANGLE1", "ANGLE2", "VELOC.", "ACCEL.", "DECEL.", "DELAY", "EXEC."};

        int cnt = 0;
        for (String aSpremenljivkeTekst : spremenljivkeTekst)
        {
            textView = new TextView(this);
            textView.setText(aSpremenljivkeTekst);
            textView.setTextSize(35);
            if (cnt == 1)
            {
                textView.setPadding(25, 25, 1, 25);
            }
            else if (cnt == 2)
            {
                textView.setPadding(25, 25, 5, 25);
            }
            else if (cnt == 4)
            {
                textView.setPadding(25, 25, 60, 25);
            }
            else if (cnt == 5)
            {
                textView.setPadding(25, 25, 45, 25);
            }
            else if (cnt == 6)
            {
                textView.setPadding(25, 25, 35, 25);
            }

            else
            {
                textView.setPadding(25, 25, 25, 25);
            }
            cnt = cnt + 1;
            varRow.addView(textView);
        }
        ////////////////////////////////////////////////////////////////////////////
        LinearLayout frame = new LinearLayout(this);
        LinearLayout.LayoutParams frameParam = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        frame.setLayoutParams(frameParam);
        frame.setOrientation(LinearLayout.VERTICAL);
        frame.addView(varRow);
        frame.addView(scrollView);
        /////////////////////////////////////////////////////
        //////////////////////////////
        LinearLayout base = new LinearLayout(this);
        base.setOrientation(LinearLayout.VERTICAL);

        LinearLayout.LayoutParams baseParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        base.setLayoutParams(baseParams);

        base.addView(topbar);
        base.addView(frame);
        /////////////////////////////////
        setContentView(base);

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                AddData();
                return null;
            }
        }.execute();


        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                populateList();
                return null;
            }
        }.execute();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                getSelected();
                return null;
            }
        }.execute();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                deleteRecipe();
                return null;
            }
        }.execute();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                newRecipe();
                return null;
            }
        }.execute();

        recName.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusable(true);
                v.setFocusableInTouchMode(true);
                return false;
            }
        });

    }


    @Override
    protected void onResume()
    {
        Log.d("EDITOR", "ON RESUME");
        super.onResume();

        listview.requestFocus();
    }

    @Override
    protected void onStop()
    {
        Log.d("EDITOR", "ON STOP");
        super.onStop();
    }

    private static void hideKeyboardFrom(Context context, View view) {
        InputMethodManager imm = (InputMethodManager) context.getSystemService(Activity.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }


    private void newRecipe()
    {
        newrec.setOnClickListener(
                new View.OnClickListener()
                {
                    @Override
                    public void onClick(View v)
                    {
                        Log.d("EDITOR", "NEW RECIPE");

                        recName.requestFocus();
                        drawTable();
                    }
                }
        );
    }
    private void AddData()
    {
        save.setOnClickListener(
                new View.OnClickListener()
                {
                    @Override
                    public void onClick(View v)
                    {
                        Log.d("EDITOR", "ADD DATA");
                        boolean skipRest1 = false;
                        boolean skipRest2;
                        boolean skipRest3;
                        boolean skipRest4 = false;
                        boolean skipRest5;
                        tableName = recName.getText().toString();
                        myDb.getTN(tableName);
                        SQLiteDatabase db = myDb.getWritableDatabase();

                        Integer idcount = 0;


                        for(int i = 1; i < recName.length(); ++i)
                            if(!Character.isAlphabetic(recName.getText().charAt(i)))
                            {
                                skipRest1 = true;
                                Toast.makeText(getApplicationContext(), "USE ONLY LETTERS FOR RECIPE NAME", Toast.LENGTH_LONG).show();
                                break;
                            }
                            else
                            {
                                skipRest1 = false;
                            }


                        if(!Character.isAlphabetic(recName.getText().charAt(0)))
                        {
                            skipRest2 = true;
                            Toast.makeText(getApplicationContext(), "RECIPE NAME MUST START WITH A LETTER", Toast.LENGTH_LONG).show();

                        }
                        else
                        {
                            skipRest2 = false;
                        }

                        if(recName.getText().length() < 2)
                        {
                            skipRest3 = true;
                            Toast.makeText(getApplicationContext(), "RECIPE NAME MUST BE LONGER THAN ONE CHARACTER", Toast.LENGTH_LONG).show();
                        }
                        else
                        {
                            skipRest3 = false;
                        }

                        for(int i = 0; i < 70; ++i)
                        {

                            if ((allET.get(i).getText().toString()).isEmpty())
                            {
                                allET.get(i).requestFocus();
                                Toast.makeText(getApplicationContext(), "CELL MUST NOT BE EMPTY", Toast.LENGTH_LONG).show();
                                skipRest4 = true;
                                break;
                            }
                            else
                            {
                                skipRest4 = false;
                            }
                        }


                        if (recName.getText().toString().isEmpty())
                        {
                            Toast.makeText(getApplicationContext(), "PLEASE INSERT RECIPE NAME", Toast.LENGTH_LONG).show();
                            skipRest5 = true;
                        }
                        else
                        {
                            skipRest5 = false;
                        }

                        if(!skipRest1 && !skipRest2 &&!skipRest3 && !skipRest4 && !skipRest5)
                        {
                            try
                            {
                                myDb.createTable(db);
                                myDb.tableCheck();
                            }
                            catch (Exception e)
                            {
                                Toast.makeText(getApplicationContext(), "PLEASE INSERT RECIPE NAME", Toast.LENGTH_LONG).show();
                            }
                            ////////////////////////////////////////// UPDATE VALUES ///////////////////////////////////////////////////////////////////
                            if(myDb.tableExists)
                            {
                                idcount = 1;
                                for(int i = 0; i < 70; i = i + 7)
                                {
                                    conditionCheck(i);

                                    if(cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7)
                                    {
                                        myDb.updateData(idcount,Integer.parseInt(allET.get(i).getText().toString()),Integer.parseInt(allET.get(i+1).getText().toString()),Integer.parseInt(allET.get(i+2).getText().toString()),Integer.parseInt(allET.get(i+3).getText().toString()),Integer.parseInt(allET.get(i+4).getText().toString()),Integer.parseInt(allET.get(i+5).getText().toString()),Integer.parseInt(allET.get(i+6).getText().toString()));
                                        idcount++;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                if(cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7)
                                {
                                    listview.requestFocus();
                                    drawTable();
                                }
                            }
                            ////////////////////////////////////////// INSERT VALUES ///////////////////////////////////////////////////////////////////
                            else
                            {
                                for(int i = 0; i < 70; i = i + 7)
                                {
                                    conditionCheck(i);

                                    if(cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7)
                                    {
                                        myDb.insertData(idcount,Integer.parseInt(allET.get(i).getText().toString()),Integer.parseInt(allET.get(i+1).getText().toString()),Integer.parseInt(allET.get(i+2).getText().toString()),Integer.parseInt(allET.get(i+3).getText().toString()),Integer.parseInt(allET.get(i+4).getText().toString()),Integer.parseInt(allET.get(i+5).getText().toString()),Integer.parseInt(allET.get(i+6).getText().toString()));
                                        idcount++;


                                    }
                                    else
                                    {
                                        break;
                                    }

                                }

                                if(cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7)
                                {
                                    listview.requestFocus();
                                    populateList();
                                    drawTable();
                                    Toast.makeText(getApplicationContext(), "RECIPE SAVED", Toast.LENGTH_LONG).show();
                                }
                                else
                                {
                                    deleteRecipe();
                                }
                            }

                        }

                        hideKeyboardFrom(context, v);

                    }
                }
        );
    }

    private void populateList()
    {
        Log.d("EDITOR", "POPULATE LIST");
            lStr.removeAll(lStr);
            myDb.getAllTables(lStr,adapter);
    }


    private void drawTable()
    {
        Log.d("EDITOR", "DRAW TABLE");
        listview.setSelector(android.R.color.transparent);
        recName.setText("");
        for(int i = 0; i < allET.size(); ++i)
        {
            allET.get(i).setText(String.valueOf(0));
            allET.get(i).clearFocus();
        }
        for(int i = 1; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            allET.get(i).clearFocus();
        }
        for(int i = 3; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            allET.get(i).clearFocus();
        }
        for(int i = 4; i < allET.size(); i = i + 7)
        {
            allET.get(i).setText(String.valueOf(100));
            allET.get(i).clearFocus();
        }

    }

    private void conditionCheck(int i)
    {
        Log.d("EDITOR", "CONDITION CHECK");
        cond1 = false;
        cond2 = false;
        cond3 = false;
        cond4 = false;
        cond5 = false;
        cond6 = false;
        cond7 = false;
        if(Integer.parseInt(allET.get(i).getText().toString()) > -1 && Integer.parseInt(allET.get(i).getText().toString()) < 181 || allET.get(i).getText().toString().isEmpty())
        {
            cond1 = true;
        }
        else
        {
            cond1 = false;
            allET.get(i).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT ANGLE1 VALUE (valid range is: 0 - 180)", Toast.LENGTH_LONG).show();
        }
        if(Integer.parseInt(allET.get(i+1).getText().toString()) > -1 && Integer.parseInt(allET.get(i+1).getText().toString()) < 181)
        {
            cond2 = true;
        }
        else
        {
            cond2 = false;
            allET.get(i+1).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT ANGLE2 VALUE (valid range is: 0 - 180)", Toast.LENGTH_LONG).show();
            Log.d("WRONG VALUE:", allET.get(i+1).getText().toString());
        }
        if(Integer.parseInt(allET.get(i+2).getText().toString()) > -1 && Integer.parseInt(allET.get(i+2).getText().toString()) < 101)
        {
            cond3 = true;
        }
        else
        {
            cond3 = false;
            allET.get(i+2).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT VELOCITY VALUE (valid range is: 0 - 100)", Toast.LENGTH_LONG).show();
            Log.d("wrong:", (allET.get(i+2).getText().toString()));
        }
        if(Integer.parseInt(allET.get(i+3).getText().toString()) > -99 && Integer.parseInt(allET.get(i+3).getText().toString()) < 30001)
        {
            cond4 = true;
        }
        else
        {
            cond4 = false;
            allET.get(i+3).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT ACCELERATION VALUE (valid range is: 100 - 20000)", Toast.LENGTH_LONG).show();

        }
        if(Integer.parseInt(allET.get(i+4).getText().toString()) > 99 && Integer.parseInt(allET.get(i+4).getText().toString()) < 30001)
        {
            cond5 = true;
        }
        else
        {
            cond5 = false;
            allET.get(i+4).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT DECCELERATION VALUE (valid range is: 100 - 20000)", Toast.LENGTH_LONG).show();
        }
        if(Integer.parseInt(allET.get(i+5).getText().toString()) > -1 && Integer.parseInt(allET.get(i+5).getText().toString()) < 1001)
        {
            cond6 = true;
        }
        else
        {
            cond6 = false;
            allET.get(i+5).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT DELAY VALUE (valid range is: 0 - 1000)", Toast.LENGTH_LONG).show();
        }
        if(Integer.parseInt(allET.get(i+6).getText().toString()) > -1 && Integer.parseInt(allET.get(i+6).getText().toString()) < 60)
        {
            cond7 = true;
        }
        else
        {
            cond7 = false;
            allET.get(i+6).requestFocus();
            Toast.makeText(getApplicationContext(), "INCORRECT EXECUTION VALUE (valid range is: 0 - 15)", Toast.LENGTH_LONG).show();
        }
    }

    private void deleteRecipe()
    {
        delete.setOnClickListener(
                new View.OnClickListener()
                {
                    @Override
                    public void onClick(View v)
                    {
                        Log.d("EDITOR", "DELETE RECIPE");
                        tableName = recName.getText().toString();
                        myDb.getTN(tableName);

                        try
                        {
                            myDb.deleteTable();
                            adapter.notifyDataSetChanged();
                            populateList();
                            drawTable();
                        }
                        catch (Exception e)
                        {
                            Toast.makeText(getApplicationContext(), "PLEASE SELECT RECIPE", Toast.LENGTH_LONG).show();
                        }
                    }
                });
    }

    private void getSelected()
    {
        listview.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
                Log.d("EDITOR", "GET SELECTED");

                String selectedFromList = (listview.getItemAtPosition(position).toString());
                recName.setText(selectedFromList);
                tableName = recName.getText().toString();
                //////////////////////////////////////////
                myDb.getTN(tableName);
                myDb.getAllData();
                myDb.tableCheck();
                for(int i = 0; i < allET.size(); ++i)
                {
                    allET.get(i).setText(String.valueOf(myDb.yData[i]));
                }
                //////////////////////////////////////////
                listview.setSelector(android.R.color.holo_blue_light);
                listview.requestFocus();
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        Log.d("EDITOR", "ON CREATE OPTIONS MENU");
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.optmenu_ed, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        Log.d("EDITOR", "ON OPTIONS ITEM SELECTED");
        int i = item.getItemId();
        Intent intent;

        if (i == R.id.back)
        {
            intent = new Intent(Editor.this, StatusMonitor.class);
            startActivity(intent);
            return super.onOptionsItemSelected(item);
        }
        else
        {
            return super.onOptionsItemSelected(item);
        }
    }
}
