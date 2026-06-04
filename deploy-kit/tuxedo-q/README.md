# Tuxedo /Q Integration - Deployment Kit

Complete deployment kit for Tuxedo /Q integration with Camel and Kafka on OpenShift.

## Overview

This deployment kit provides automated scripts and templates to deploy the complete Tuxedo /Q integration stack:

```
WebUI → Tuxedo /Q (REST API) → Camel (REST polling) → Kafka
```

## Prerequisites

- OpenShift 4.x cluster
- `oc` CLI installed and configured
- Kafka cluster already deployed (see `../kafka/` for deployment)
- Oracle Tuxedo base image access (optional, can use public mirror)

## Architecture

```
┌─────────────┐
│   WebUI     │ (Quarkus + React)
│  Namespace  │
└──────┬──────┘
       │ HTTP POST /api/send
       ↓
┌─────────────┐
│  Tuxedo /Q  │ (Python REST API + fallback queue)
│  Namespace  │
└──────┬──────┘
       │ GET /dequeue (polling 1s)
       ↓
┌─────────────┐
│    Camel    │ (Quarkus + Camel)
│  Namespace  │
└──────┬──────┘
       │ Kafka Producer
       ↓
┌─────────────┐
│    Kafka    │
│  Namespace  │
└─────────────┘
```

## Quick Start

### Option 1: Deploy Everything (Recommended)

```bash
cd scripts
./deploy-all.sh
```

Default namespaces:
- Tuxedo: `demo-tuxedo`
- WebUI: `demo-webui`
- Camel: `demo-camel`
- Kafka: `demo-kafka` (must exist)

### Option 2: Deploy Components Separately

```bash
# 1. Deploy Tuxedo /Q
./scripts/deploy-tuxedo.sh demo-tuxedo

# 2. Deploy WebUI
./scripts/deploy-webui.sh demo-webui demo-tuxedo

# 3. Deploy Camel
./scripts/deploy-camel.sh demo-camel demo-kafka
```

## Deployment Scripts

### deploy-all.sh

Complete deployment of all components.

**Usage**:
```bash
./deploy-all.sh [tuxedo-ns] [webui-ns] [camel-ns] [kafka-ns] [git-repo] [git-branch]
```

**Example**:
```bash
./deploy-all.sh my-tuxedo my-webui my-camel my-kafka \
  https://github.com/yourname/tuxedo-to-camel.git \
  feature/tuxedo-q-integration
```

### deploy-tuxedo.sh

Deploys Tuxedo /Q with REST API server.

**Usage**:
```bash
./deploy-tuxedo.sh [namespace] [git-repo] [git-branch] [base-image]
```

**Features**:
- Tuxedo base image import
- Python REST API server (`tuxedo-q-server.py`)
- In-memory fallback queue
- `/enqueue`, `/dequeue`, `/health` endpoints

### deploy-webui.sh

Deploys Quarkus WebUI.

**Usage**:
```bash
./deploy-webui.sh [namespace] [tuxedo-namespace] [git-repo] [git-branch]
```

**Features**:
- React frontend
- Quarkus backend
- REST client to Tuxedo

### deploy-camel.sh

Deploys Camel integration.

**Usage**:
```bash
./deploy-camel.sh [namespace] [kafka-namespace] [git-repo] [git-branch]
```

**Features**:
- Timer-based REST polling (1s interval)
- JSON transformation
- Kafka producer

## Configuration

### Environment Variables

**Tuxedo**:
- `TUXEDO_QUEUE`: Queue name (default: `DEMO_QUEUE`)

**WebUI**:
- `TUXEDO_URL`: Tuxedo service URL

**Camel**:
- `kafka.brokers`: Kafka bootstrap servers

### Customization

Edit YAML templates in:
- `templates/` - Tuxedo templates
- `webui/` - WebUI templates
- `camel/` - Camel templates

Placeholders:
- `NAMESPACE_PLACEHOLDER` - Target namespace
- `GIT_REPO_PLACEHOLDER` - Git repository URL
- `GIT_BRANCH_PLACEHOLDER` - Git branch
- `TUXEDO_URL_PLACEHOLDER` - Tuxedo service URL
- `KAFKA_BROKERS_PLACEHOLDER` - Kafka brokers

## Testing

### 1. Check Deployment Status

```bash
oc get pods -n demo-tuxedo
oc get pods -n demo-webui
oc get pods -n demo-camel
```

### 2. Get WebUI URL

```bash
oc get route webui -n demo-webui -o jsonpath='{.spec.host}'
```

### 3. Send Test Message

```bash
curl -X POST https://$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')/api/send \
  -H "Content-Type: text/plain" \
  -d "Test message from Tuxedo /Q" -k
```

### 4. Check Logs

**Tuxedo** (enqueue):
```bash
oc logs -n demo-tuxedo -l app=tuxedo-msgsvc --tail=20
```

**Camel** (dequeue + Kafka):
```bash
oc logs -n demo-camel -l app=camel-kafka-bridge --tail=20
```

### 5. Verify Kafka Messages

```bash
oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \
  --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 -- \
  bin/kafka-console-consumer.sh \
  --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
  --topic demo-messages \
  --from-beginning \
  --max-messages 5
```

## Troubleshooting

### Tuxedo Pod Not Starting

**Check logs**:
```bash
oc logs -n demo-tuxedo -l app=tuxedo-msgsvc
```

**Common issues**:
- Base image pull failure (Oracle credentials required)
- Git repo access denied
- Insufficient resources

### WebUI Cannot Connect to Tuxedo

**Check service**:
```bash
oc get svc -n demo-tuxedo
```

**Test connectivity**:
```bash
oc exec -n demo-webui <webui-pod> -- \
  curl http://tuxedo-msgsvc.demo-tuxedo.svc.cluster.local:8080/health
```

### Camel Not Polling

**Check environment**:
```bash
oc get deployment camel-kafka-bridge -n demo-camel -o yaml | grep -A5 env
```

**Verify Tuxedo route**:
```bash
oc exec -n demo-camel <camel-pod> -- \
  curl http://tuxedo-msgsvc.demo-tuxedo.svc.cluster.local:8080/dequeue
```

## Cleanup

```bash
oc delete project demo-tuxedo demo-webui demo-camel
```

## Directory Structure

```
deploy-kit/tuxedo-q/
├── README.md              # This file
├── QUICKSTART.md          # Quick start guide
├── scripts/
│   ├── deploy-all.sh      # All-in-one deployment
│   ├── deploy-tuxedo.sh   # Tuxedo deployment
│   ├── deploy-webui.sh    # WebUI deployment
│   └── deploy-camel.sh    # Camel deployment
├── templates/             # Tuxedo YAML templates
│   ├── 01-namespace.yaml
│   ├── 02-base-imagestream.yaml
│   ├── 03-base-buildconfig.yaml
│   ├── 04-msgsvc-imagestream.yaml
│   ├── 05-msgsvc-buildconfig.yaml
│   ├── 06-deployment.yaml
│   ├── 07-service.yaml
│   └── 08-route.yaml
├── webui/                 # WebUI YAML templates
│   ├── 01-namespace.yaml
│   ├── 02-deployment.yaml
│   ├── 03-service.yaml
│   ├── 04-route.yaml
│   └── 05-buildconfig.yaml
└── camel/                 # Camel YAML templates
    ├── 01-namespace.yaml
    ├── 02-imagestream.yaml
    ├── 03-buildconfig.yaml
    ├── 04-deployment.yaml
    └── 05-service.yaml
```

## Next Steps

- Deploy Kafka if not already present (see `../kafka/`)
- Configure Kafka Console for message monitoring (see `../kafka/`)
- Review E2E test results in `../../E2E_TEST_RESULTS.md`
- For production: implement actual Tuxedo QSPACE (see `../../TASK_TUXEDO_Q_INTEGRATION.md`)

## References

- Main Project: `../../README.md`
- E2E Test Results: `../../E2E_TEST_RESULTS.md`
- Progress Tracking: `../../PROGRESS_TUXEDO_Q.md`
- AMQ Deployment: `../amq/`
- Kafka Deployment: `../kafka/`
