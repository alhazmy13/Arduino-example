package net.alhazmy13.rip;

import retrofit2.Call;
import retrofit2.http.GET;

/**
 * Created by Alhazmy13 on 4/28/16.
 * Rip
 */
public interface Api {
    @GET("/api/data")
    Call<DataModel> getData();
}
