package com.example;

import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import org.jboss.logging.Logger;

import java.time.Instant;
import java.util.Map;

@Path("/api")
@Produces(MediaType.APPLICATION_JSON)
public class MessageResource {

    private static final Logger LOG = Logger.getLogger(MessageResource.class);

    @POST
    @Path("/send")
    @Consumes(MediaType.TEXT_PLAIN)
    public Response sendMessage(String message) {
        LOG.infof("[WebUI] Received: %s", message);

        try {
            // TODO: TuxedoClientを使ってメッセージ送信
            // 現時点ではログ出力のみ（Tuxedoサービスができたら実装）
            LOG.infof("[WebUI] Sent to Tuxedo: %s", message);

            return Response.ok(Map.of(
                "status", "success",
                "message", "Message sent to Tuxedo",
                "timestamp", Instant.now().toString()
            )).build();

        } catch (Exception e) {
            LOG.errorf(e, "[WebUI] Failed to send message: %s", message);

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
