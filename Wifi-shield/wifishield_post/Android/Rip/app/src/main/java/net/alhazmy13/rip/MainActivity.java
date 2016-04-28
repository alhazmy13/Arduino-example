package net.alhazmy13.rip;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends AppCompatActivity {
    private TextView lastSeen;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        lastSeen = (TextView) findViewById(R.id.lastSeen);
    }

    @Override
    protected void onStart() {
        super.onStart();
        getDataFromNetwork();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main,menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if(item.getItemId() == R.id.refresh){
            getDataFromNetwork();
        }
        return super.onOptionsItemSelected(item);
    }

    private void getDataFromNetwork() {
        App.getRetrofit().create(Api.class).getData().enqueue(new Callback<DataModel>() {
            @Override
            public void onResponse(Call<DataModel> call, Response<DataModel> response) {
                if(response.code() == 200){
                    updateView(response.body());
                }
            }



            @Override
            public void onFailure(Call<DataModel> call, Throwable t) {
                Toast.makeText(MainActivity.this, "There're an error, please try again later", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void updateView(DataModel body) {
        if(body == null)
            return;
        lastSeen.setText(body.getData().get(body.getData().size()-1).getDate());
    }
}
