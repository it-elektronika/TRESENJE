package com.it_elektronika.luka.tresenje;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.DatabaseUtils;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;
import android.widget.ArrayAdapter;
import java.util.ArrayList;


class DataBaseHelper extends SQLiteOpenHelper {
    private static final String DATABASE_NAME = "new.db";
    private static  String TABLE_NAME;
    public Integer[] yData;
    boolean tableExists;

    public DataBaseHelper(Context context){
        super(context, DATABASE_NAME, null,1);
    }

    @Override
    public void onCreate(SQLiteDatabase db){
        //db.execSQL(" CREATE TABLE default (ID INTEGER PRIMARY KEY AUTOINCREMENT, ANGLE1 INTEGER, ANGLE2 INTEGER, VELOCITY INTEGER, ACCELERATION INTEGER, DECCELERATION INTEGER, DELAY INTEGER, EXECUTION INTEGER);");
        //Log.v("TABLE CREATED:", TABLE_NAME);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){
        db.execSQL("DROP TABLE IF EXISTS " +  TABLE_NAME);
        onCreate(db);
    }

    public void getTN(String tableName)
    {
        TABLE_NAME = tableName;
        Log.v("Name is:",TABLE_NAME);

    }


    public void createTable(SQLiteDatabase db)
    {
        db.execSQL(" CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, ANGLE1 INTEGER, ANGLE2 INTEGER, VELOCITY INTEGER, ACCELERATION INTEGER, DECCELERATION INTEGER, DELAY INTEGER, EXECUTION INTEGER);");
        Log.v("TABLE CREATED:", TABLE_NAME);
    }

    public void tableCheck(){
        Log.d("function:", "tablexists");

        tableExists = false;
        SQLiteDatabase db = this.getWritableDatabase();

        if ((DatabaseUtils.queryNumEntries(db, TABLE_NAME)) < 1)
        {
            Log.d("number of rows:", String.valueOf(DatabaseUtils.queryNumEntries(db, TABLE_NAME)));
            tableExists = false;
        }
        else
        {
            Log.d("number of rows:", String.valueOf(DatabaseUtils.queryNumEntries(db, TABLE_NAME)));
            tableExists = true;
        }
    }

    public void insertData(Integer idcount, Integer angle1, Integer angle2, Integer velocity, Integer acceleration, Integer decceleration, Integer delay, Integer execution){

        SQLiteDatabase db = this.getWritableDatabase();
        Log.d("TABLE EXISTS:", String.valueOf(tableExists));
        ContentValues contentValues = new ContentValues();
        contentValues.put("ANGLE1", angle1);
        contentValues.put("ANGLE2", angle2);
        contentValues.put("VELOCITY", velocity);
        contentValues.put("ACCELERATION", acceleration);
        contentValues.put("DECCELERATION", decceleration);
        contentValues.put("DELAY", delay);
        contentValues.put("EXECUTION", execution);
        long result;

        result = db.insert(TABLE_NAME, null,contentValues);
        Log.d("message", "Inserting values");
    }

    public void updateData(Integer idcount, Integer angle1, Integer angle2, Integer velocity, Integer acceleration, Integer decceleration, Integer delay, Integer execution){

        SQLiteDatabase db = this.getWritableDatabase();
        Log.d("TABLE EXISTS:", String.valueOf(tableExists));

        ContentValues contentValues = new ContentValues();
        contentValues.put("ANGLE1", angle1);
        contentValues.put("ANGLE2", angle2);
        contentValues.put("VELOCITY", velocity);
        contentValues.put("ACCELERATION", acceleration);
        contentValues.put("DECCELERATION", decceleration);
        contentValues.put("DELAY", delay);
        contentValues.put("EXECUTION", execution);

        db.update(TABLE_NAME, contentValues, "ID = ?", new String[]{String.valueOf(idcount)});
        Log.d("message", String.valueOf(angle1));
        Log.d("message", String.valueOf(idcount));
        Log.d("message", "Updating values");

    }



    public void getAllData()
    {
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor res = db.rawQuery("select * from " + TABLE_NAME, null);
        yData = new Integer[70];
        Integer countYData = 0;
        if(res.moveToFirst())
        {
            String[] columnNames = {"ANGLE1", "ANGLE2", "VELOCITY", "ACCELERATION", "DECCELERATION", "DELAY", "EXECUTION"};
            while ( !res.isAfterLast() )
            {

                for (String columnName : columnNames) {
                    yData[countYData] = res.getInt(res.getColumnIndex(columnName));
                    countYData++;
                    //Log.d("iteration: ", String.valueOf(i));
                    //Log.d("data: ", yData[i]);
                    //Log.d("res pos: ", String.valueOf(res.getPosition()));
                    //Log.d("col ind: ", String.valueOf(res.getColumnIndex(columnNames[i])));
                }

                Log.d("outer", "end of iteration");
                res.moveToNext();
            }
        }
        for (int i = 0; i < 70; ++i)
        {
            Log.d("ydata:", String.valueOf(yData[i]));
        }
        res.close();
    }

    public void getAllTables(ArrayList arrayList, ArrayAdapter adapter) {
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor result = db.rawQuery("SELECT name FROM sqlite_master WHERE type='table'", null);
        if (result.moveToFirst()) {
            while ( !result.isAfterLast() ) {

                if(!(result.getString( result.getColumnIndex("name"))).equals("android_metadata") && !(result.getString( result.getColumnIndex("name"))).equals("sqlite_sequence"))
                {
                    arrayList.add(result.getString( result.getColumnIndex("name")));
                    adapter.notifyDataSetChanged();
                }

                result.moveToNext();
            }
        }
    }


    public void deleteTable(){
        SQLiteDatabase db = this.getWritableDatabase();
        Log.d("table to be deleted:", TABLE_NAME);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);

    }
}
