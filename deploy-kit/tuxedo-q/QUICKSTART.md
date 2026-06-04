# Tuxedo /Q Integration - Quick Start Guide

Get the complete Tuxedo /Q integration running in **5 minutes**.

## Prerequisites Check

```bash
# 1. Check oc CLI
oc version

# 2. Check login
oc whoami

# 3. Check Kafka is deployed
oc get kafka -n demo-kafka
```

## Step 1: Clone Repository

```bash
git clone https://github.com/kamorisan/tuxedo-to-camel.git
cd tuxedo-to-camel/deploy-kit/tuxedo-q
```

## Step 2: Deploy Everything

```bash
cd scripts
./deploy-all.sh
```

**Press `y` when prompted.**

This will deploy:
1. ✅ Tuxedo /Q (`demo-tuxedo` namespace)
2. ✅ WebUI (`demo-webui` namespace)
3. ✅ Camel (`demo-camel` namespace)

**Estimated time**: 5-10 minutes (depending on build times)

## Step 3: Wait for Builds

```bash
# Watch Tuxedo build
oc logs -f bc/tuxedo-msgsvc -n demo-tuxedo

# Watch WebUI build (in another terminal)
oc logs -f bc/webui -n demo-webui

# Watch Camel build (in another terminal)
oc logs -f bc/camel-kafka-bridge -n demo-camel
```

## Step 4: Verify Deployment

```bash
# Check all pods are running
oc get pods -n demo-tuxedo
oc get pods -n demo-webui
oc get pods -n demo-camel
```

Expected output:
```
NAME                             READY   STATUS    RESTARTS   AGE
tuxedo-msgsvc-xxxx               1/1     Running   0          5m
webui-xxxx                       1/1     Running   0          4m
camel-kafka-bridge-xxxx          1/1     Running   0          3m
```

## Step 5: Test E2E Flow

### 5.1 Get WebUI URL

```bash
echo "https://$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')"
```

### 5.2 Send Test Message

```bash
curl -X POST https://$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')/api/send \
  -H "Content-Type: text/plain" \
  -d "Hello from Tuxedo /Q!" -k
```

Expected response:
```json
{
  "status": "success",
  "message": "Message sent to Tuxedo successfully",
  "tuxedoResponse": "{\"status\": \"success\", \"message\": \"Message enqueued\", \"queue\": \"DEMO_QUEUE\"}",
  "timestamp": "2026-06-04T..."
}
```

### 5.3 Check Logs

**Tuxedo** (should show enqueue):
```bash
oc logs -n demo-tuxedo -l app=tuxedo-msgsvc --tail=5
```

Output:
```
[TuxedoQ-Server] Received for enqueue: Hello from Tuxedo /Q!
[TuxedoQ-Server] Enqueued to in-memory fallback queue (size: 1)
[TuxedoQ-Server] Dequeued from in-memory fallback queue (remaining: 0)
```

**Camel** (should show Kafka send):
```bash
oc logs -n demo-camel -l app=camel-kafka-bridge --tail=5
```

Output:
```
INFO [Camel] Sending to Kafka: {"message":"Hello from Tuxedo /Q!",...}
INFO [Camel] Sent to Kafka topic: demo-messages
```

### 5.4 Verify in Kafka

```bash
oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \
  --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 -- \
  bin/kafka-console-consumer.sh \
  --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
  --topic demo-messages \
  --from-beginning \
  --max-messages 1
```

## Complete Flow Summary

```
✅ WebUI → Tuxedo /Q (enqueue) → Camel (dequeue) → Kafka
```

**Latency**: < 3 seconds end-to-end

## Next Steps

### Open WebUI in Browser

```bash
open "https://$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')"
```

### Access Kafka Console

```bash
# Get Kafka Console URL
oc get route -n demo-kafka | grep console
```

### Monitor Logs in Real-Time

```bash
# Terminal 1: Tuxedo
oc logs -n demo-tuxedo -l app=tuxedo-msgsvc -f

# Terminal 2: Camel
oc logs -n demo-camel -l app=camel-kafka-bridge -f
```

## Troubleshooting

### Build Failed

```bash
# Check build logs
oc logs bc/tuxedo-msgsvc -n demo-tuxedo --tail=50
```

**Fix**: Check Git repo access, retry build
```bash
oc start-build tuxedo-msgsvc -n demo-tuxedo --follow
```

### Pod Not Running

```bash
# Describe pod
oc describe pod -n demo-tuxedo -l app=tuxedo-msgsvc
```

**Common issues**:
- Image pull backoff → Check base image availability
- CrashLoopBackOff → Check pod logs

### No Message in Kafka

**Check each component**:

1. WebUI → Tuxedo:
```bash
curl http://$(oc get route tuxedo-msgsvc -n demo-tuxedo -o jsonpath='{.spec.host}')/health
```

2. Camel → Tuxedo:
```bash
oc exec -n demo-camel $(oc get pod -n demo-camel -l app=camel-kafka-bridge -o name) -- \
  curl http://tuxedo-msgsvc.demo-tuxedo.svc.cluster.local:8080/dequeue
```

3. Camel → Kafka:
```bash
oc logs -n demo-camel -l app=camel-kafka-bridge | grep Kafka
```

## Cleanup

```bash
oc delete project demo-tuxedo demo-webui demo-camel
```

## Custom Deployment

### Use Your Own Git Repo

```bash
./deploy-all.sh demo-tuxedo demo-webui demo-camel demo-kafka \
  https://github.com/YOUR_USERNAME/tuxedo-to-camel.git \
  YOUR_BRANCH_NAME
```

### Use Different Namespaces

```bash
./deploy-all.sh my-tux my-ui my-camel my-kafka
```

## Architecture Details

For detailed architecture, see:
- [README.md](./README.md) - Complete documentation
- [../../E2E_TEST_RESULTS.md](../../E2E_TEST_RESULTS.md) - Test results
- [../../PROGRESS_TUXEDO_Q.md](../../PROGRESS_TUXEDO_Q.md) - Implementation progress

## Support

- Issues: https://github.com/kamorisan/tuxedo-to-camel/issues
- Docs: `../../README.md`

---

**You're all set!** 🎉

The complete Tuxedo /Q integration is now running on your OpenShift cluster.
