package cyberphysical.cyber;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private static Context context;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        BroadcastReceiver broadcastReceiver = new WifiReciever();

        context = getApplicationContext();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.SUPPLICANT_CONNECTION_CHANGE_ACTION);
        context.registerReceiver(broadcastReceiver, intentFilter);

        String url = "192.168.4.1";
        WebView view = (WebView) this.findViewById(R.id.webview);


        do
        {
            //displays error message until correct wifi connectd
            int toastTime = 60000;//length till reset
            final Toast toast;//error message
            toast = Toast.makeText(context,"The Wifi network is not connected " +
                    "\n please connect to Hello_IoT wifi network" +
                    "\n Retrying in 60 seconds",Toast.LENGTH_LONG);

            CountDownTimer countDownTimer;
            //count down for excitement
            countDownTimer = new CountDownTimer(toastTime,1000) {
                @Override
                public void onTick(long millisUntilFinished) {
                    toast.show();
                }

                @Override
                public void onFinish() {
                    toast.cancel();
                }
            };

            toast.show();//display message
            countDownTimer.start();//start the clock
        }while(!new WifiReciever().isConnectedToBoard());

        view.getSettings().setJavaScriptEnabled(true);
        view.loadUrl(url);//load the web page

    }


    //checks for the correct connection
    public class WifiReciever extends BroadcastReceiver
    {

        @Override
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();
            if(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION.equals(action))
            {
                SupplicantState state = intent.getParcelableExtra(WifiManager.EXTRA_NEW_STATE);
                if (SupplicantState.isValidState(state)
                        && state == SupplicantState.COMPLETED)
                {
                    boolean connected = isConnectedToBoard();
                }
            }
        }

        private boolean isConnectedToBoard()
        {
            boolean conn = false;

            //the wemos mac address
            String wemosMAC = "5C:CF:7F:C8:D9:2A";

            WifiManager wm = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            WifiInfo wifiInfo = wm.getConnectionInfo();
            if(wifiInfo != null)
            {
                String bssid = wifiInfo.getBSSID();
                conn = wemosMAC.equals(bssid);
            }

            return conn;
        }
    }
}

/*sources:
http://stackoverflow.com/questions/9353005/
android-wifi-how-to-detect-when-specific-wifi-connection-is-available
*/