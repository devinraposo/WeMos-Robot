package cyberphysical.cyber.Database;

import android.os.AsyncTask;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by Mr.Big on 11/22/2016.
 */

public class Connection extends AsyncTask<String,Void, String>
{

    @Override
    protected String doInBackground(String... urls) {
        try
        {
            downloadData(urls[0]);
        } catch (IOException e){
            return "unable to get webpage";
        }
        return null;
    }

    private void downloadData(String url) throws IOException
    {
        InputStream is = null;

        int len = 500;

        try
        {
            //get url connection
            URL myUrl = new URL(url);
            HttpURLConnection conn = (HttpURLConnection) myUrl.openConnection();
            conn.setReadTimeout(1000);
            conn.setConnectTimeout(1000);
            conn.setRequestMethod("GET");
            conn.setDoInput(true);

            //query
            conn.connect();
            int response = conn.getResponseCode();
            is = conn.getInputStream();

            //convert is to string
            String contentString = readIt(is,len);

        } finally {
            if (is != null)
            {
                is.close();
            }
        }

    }//end method

    private String readIt(InputStream is, int len) throws IOException, UnsupportedEncodingException
    {
        Reader reader = null;
        reader = new InputStreamReader(is,"UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return  new String(buffer);
    }

}
