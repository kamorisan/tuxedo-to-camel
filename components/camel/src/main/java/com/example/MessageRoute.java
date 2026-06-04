package com.example;

import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.model.dataformat.JsonLibrary;
import jakarta.enterprise.context.ApplicationScoped;

import java.time.Instant;
import java.util.Map;
import java.util.UUID;

@ApplicationScoped
public class MessageRoute extends RouteBuilder {

    @Override
    public void configure() throws Exception {

        // Route: AMQ Queue -> Transform to JSON -> Kafka
        from("amqp:queue:TUXEDO.OUT?subscriptionDurable=false")
            .routeId("amq-to-kafka")
            .log("[Camel] Consumed from queue: ${body}")
            .process(exchange -> {
                String originalMessage = exchange.getIn().getBody(String.class);

                Map<String, Object> jsonMessage = Map.of(
                    "id", UUID.randomUUID().toString(),
                    "timestamp", Instant.now().toString(),
                    "message", originalMessage,
                    "source", "tuxedo"
                );

                exchange.getIn().setBody(jsonMessage);
            })
            .marshal().json(JsonLibrary.Jackson)
            .log("[Camel] Transformed: ${body}")
            .to("kafka:demo-messages?brokers={{kafka.brokers}}")
            .log("[Camel] Sent to Kafka topic: demo-messages");
    }
}
