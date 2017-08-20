package com.lee.delpath;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    String TAG = "Lee";
    String mCustPath = "/sdcard/test/";
    JniTools mjJNI  = new JniTools();
    Button removeButton;
    MyOnClickListener listener = new MyOnClickListener();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        removeButton = (Button) findViewById(R.id.btn_remove);
        removeButton.setOnClickListener(listener);
    }

    class MyOnClickListener implements View.OnClickListener {
       @Override
        public void onClick(View view) {
            switch (view.getId()) {
                case R.id.btn_remove:
                    removeCustPath();
                    break;
            }
        }
    }

    public boolean removeCustPath(){
        boolean isRemoved = false;
        isRemoved =  mjJNI.removePath(mCustPath);
        Log.d(TAG,"remove:" + isRemoved);
        return isRemoved;
    }
}
