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

        // Route: Timer -> Tuxedo /Q (REST polling) -> Transform to JSON -> Kafka
        from("timer:tuxedo-q-poll?period=1000&fixedRate=true")
            .routeId("tuxedoq-to-kafka")
            .setHeader("CamelHttpMethod", constant("GET"))
            .to("http://tuxedo-msgsvc.demo-tuxedo-c.svc.cluster.local:8080/dequeue?bridgeEndpoint=true&throwExceptionOnFailure=false")
            // Parse JSON response
            .unmarshal().json(JsonLibrary.Jackson, Map.class)
            .process(exchange -> {
                @SuppressWarnings("unchecked")
                Map<String, Object> response = exchange.getIn().getBody(Map.class);
                String message = (String) response.get("message");

                // Only process if message is not null
                if (message != null && !message.isEmpty()) {
                    Map<String, Object> jsonMessage = Map.of(
                        "id", UUID.randomUUID().toString(),
                        "timestamp", Instant.now().toString(),
                        "message", message,
                        "source", "tuxedo-q",
                        "queue", response.getOrDefault("queue", "DEMO_QUEUE")
                    );
                    exchange.getIn().setBody(jsonMessage);
                    exchange.getIn().setHeader("hasMessage", true);
                } else {
                    // No message - skip Kafka send
                    exchange.getIn().setHeader("hasMessage", false);
                }
            })
            .choice()
                .when(header("hasMessage").isEqualTo(true))
                    .marshal().json(JsonLibrary.Jackson)
                    .log("[Camel] Sending to Kafka: ${body}")
                    .to("kafka:demo-messages?brokers={{kafka.brokers}}")
                    .log("[Camel] Sent to Kafka topic: demo-messages")
            .end();
    }
}
