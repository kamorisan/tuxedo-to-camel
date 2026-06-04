package com.example;

import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import org.eclipse.microprofile.rest.client.inject.RegisterRestClient;

@Path("/MSGSVC")
@RegisterRestClient(configKey = "tuxedo")
public interface TuxedoClient {

    @POST
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    TuxedoResponse sendMessage(String message);

    class TuxedoResponse {
        public String status;
        public String message;
    }
}
