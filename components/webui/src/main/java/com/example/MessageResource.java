package com.example;

import jakarta.inject.Inject;
import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import org.eclipse.microprofile.rest.client.inject.RestClient;
import org.jboss.logging.Logger;

import java.time.Instant;
import java.util.Map;

@Path("/api")
@Produces(MediaType.APPLICATION_JSON)
public class MessageResource {

    private static final Logger LOG = Logger.getLogger(MessageResource.class);

    @Inject
    @RestClient
    TuxedoClient tuxedoClient;

    @POST
    @Path("/send")
    @Consumes(MediaType.TEXT_PLAIN)
    public Response sendMessage(String message) {
        LOG.infof("[WebUI] Received: %s", message);

        try {
            // Send message to Tuxedo
            String tuxedoResponse = tuxedoClient.sendMessage(message);
            LOG.infof("[WebUI] Tuxedo response: %s", tuxedoResponse);

            return Response.ok(Map.of(
                "status", "success",
                "message", "Message sent to Tuxedo successfully",
                "tuxedoResponse", tuxedoResponse,
                "timestamp", Instant.now().toString()
            )).build();

        } catch (Exception e) {
            LOG.errorf(e, "[WebUI] Failed to send message to Tuxedo: %s", message);

            return Response.serverError().entity(Map.of(
                "status", "error",
                "message", "Failed to connect to Tuxedo service: " + e.getMessage(),
                "timestamp", Instant.now().toString()
            )).build();
        }
    }

    @GET
    @Path("/health")
    public Response health() {
        return Response.ok(Map.of(
            "status", "healthy",
            "timestamp", Instant.now().toString()
        )).build();
    }
}
