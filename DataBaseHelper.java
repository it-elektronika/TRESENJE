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
        tableExists = false;
        SQLiteDatabase db = this.getWritableDatabase();

        if ((DatabaseUtils.queryNumEntries(db, TABLE_NAME)) < 1)
        {
            tableExists = false;
        }
        else
        {;
            tableExists = true;
        }
    }

    public void insertData(Integer idcount, Integer angle1, Integer angle2, Integer velocity, Integer acceleration, Integer decceleration, Integer delay, Integer execution){

        SQLiteDatabase db = this.getWritableDatabase();
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

                }
                res.moveToNext();
            }
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
                    arrayList.add(result.getString(result.getColumnIndex("name")));
                    adapter.notifyDataSetChanged();
                }

                result.moveToNext();
            }
        }
        result.close();
    }


    public void deleteTable(){
        SQLiteDatabase db = this.getWritableDatabase();
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);

    }
}
