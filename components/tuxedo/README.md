# Tuxedo Message Service

Oracle Tuxedo based message service that receives messages via WebService Listener (WSL) and forwards them to Red Hat AMQ queue.

## Architecture

```
[WebUI] 
  ↓ HTTP POST :8080
[WSL (Tuxedo WebService Listener)]
  ↓ Tuxedo ATMI
[MSGSVC (Custom Tuxedo Service)]
  ↓ AMQP (qpid-proton)
[Red Hat AMQ Queue: TUXEDO.OUT]
  ↓
[Apache Camel] → [Kafka]
```

## Components

### Source Files

- `src/MSGSVC.c` - Main Tuxedo service implementation
- `src/amqclient.c` - AMQP client library using qpid-proton
- `src/amqclient.h` - AMQP client header

### Configuration

- `config/ubbconfig` - Tuxedo UBB configuration file
  - Defines MSGSVC service
  - Configures WSL to listen on port 8080

### Scripts

- `scripts/boot.sh` - Boot Tuxedo application
- `scripts/shutdown.sh` - Shutdown Tuxedo application
- `scripts/entrypoint.sh` - Container entrypoint script

### Build

- `Makefile` - Build configuration for Tuxedo buildserver
- `Containerfile` - Container image definition

## Prerequisites

To build this container image, you need:

1. **Oracle Tuxedo 22.1.1.0.0 base image**
   ```bash
   # Build from /Users/kamori/vscode/customer/japan-post/docker-images/OracleTuxedo/core/dockerfiles/
   cd /Users/kamori/vscode/customer/japan-post/docker-images/OracleTuxedo/core/dockerfiles
   ./buildContainerImage.sh -v 22.1.1.0.0
   
   # Tag as latest
   podman tag oracle/tuxedo:22.1.1.0.0 oracle/tuxedo:latest
   ```

## Building the Container Image

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo

# Build with podman
podman build -f Containerfile -t tuxedo-msgsvc:latest .
```

## Running Locally

```bash
# Run container
podman run -d \
  --name tuxedo-msgsvc \
  -p 8080:8080 \
  -e AMQ_HOST=amq-broker.demo-tuxedo.svc.cluster.local \
  -e AMQ_PORT=5672 \
  -e AMQ_QUEUE=TUXEDO.OUT \
  tuxedo-msgsvc:latest

# Check logs
podman logs -f tuxedo-msgsvc

# Check Tuxedo status
podman exec tuxedo-msgsvc tmadmin -r << EOF
psr
psc
quit
EOF

# Test message sending (requires AMQ running)
curl -X POST http://localhost:8080/MSGSVC \
  -H "Content-Type: text/plain" \
  -d "Hello from Tuxedo!"
```

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `AMQ_HOST` | `amq-broker.demo-tuxedo.svc.cluster.local` | AMQ broker hostname |
| `AMQ_PORT` | `5672` | AMQ broker port |
| `AMQ_QUEUE` | `TUXEDO.OUT` | AMQ queue name |

## OpenShift Deployment

See `../../openshift/tuxedo/` for Kubernetes manifests.

## Service Details

### MSGSVC Service

**Function**: Receives messages and forwards to AMQ

**Input**: Plain text message (STRING buffer)

**Output**: Same message (success) or error

**Flow**:
1. Receive message via Tuxedo ATMI (called by WSL)
2. Log message: `[Tuxedo] Received: {message}`
3. Send to AMQ queue via AMQP
4. Log: `[Tuxedo] Message queued to AMQ: {message}`
5. Return success to caller

### WSL Configuration

- **Listen Address**: `//0.0.0.0:8080`
- **Min Instances**: 1
- **Max Instances**: 5
- **Request Timeout**: 10 seconds

**HTTP Endpoint**: `POST http://{host}:8080/MSGSVC`

## Logs

Tuxedo logs are written to `ULOG` file in the application directory.

Key log messages:
- `[Tuxedo] MSGSVC initializing...` - Service startup
- `[Tuxedo] Received: {message}` - Message received
- `[Tuxedo] Message queued to AMQ: {message}` - Message sent to AMQ
- `[Tuxedo] MSGSVC shutting down...` - Service shutdown

## Troubleshooting

### Service fails to start

Check AMQ connection:
```bash
# Inside container
telnet $AMQ_HOST $AMQ_PORT
```

### Messages not reaching AMQ

Check qpid-proton library:
```bash
podman exec tuxedo-msgsvc ldd /u01/oracle/user_projects/tuxdemo/MSGSVC | grep proton
```

### WSL not responding

Check WSL process:
```bash
podman exec tuxedo-msgsvc tmadmin -r << EOF
psr
quit
EOF
# Look for WSL process
```

## License

Oracle Tuxedo is proprietary software. Ensure you have appropriate licenses.

## References

- [Oracle Tuxedo Documentation](https://docs.oracle.com/en/middleware/tuxedo/)
- [qpid-proton Documentation](https://qpid.apache.org/proton/)
- [Red Hat AMQ Documentation](https://access.redhat.com/documentation/en-us/red_hat_amq/)
