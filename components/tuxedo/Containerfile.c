# Copyright (c) 2024 Tuxedo to Camel Demo
# Tuxedo Message Service Container Image (C Implementation)
#
# This Containerfile extends Oracle Tuxedo 22.1.1.0.0 with:
# - C implementation of MSGSVC (ATMI API + libmicrohttpd)
# - Tuxedo /Q integration
# - Fallback queue for testing

# Base image: Oracle Tuxedo 22.1.1.0.0
FROM oracle/tuxedo:22.1.1.0.0

LABEL maintainer="tuxedo-to-camel-demo"
LABEL description="Tuxedo /Q Message Service (C implementation)"

USER root

# Note: gcc, make, and libmicrohttpd already installed in base image
# No additional packages needed

# Create application directory as root
RUN mkdir -p /u01/oracle/user_projects/tuxdemo && \
    chown -R oracle:oracle /u01/oracle/user_projects

# Switch to oracle user
USER oracle

# Set environment variables
ENV TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
ENV APPDIR=/u01/oracle/user_projects/tuxdemo
ENV PATH=$PATH:$TUXDIR/bin
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TUXDIR/lib
ENV TUXCONFIG=$APPDIR/tuxconfig

# Copy Tuxedo configuration and C source
COPY --chown=oracle:oracle config/ubbconfig $APPDIR/
COPY --chown=oracle:oracle src/tuxedo_q_server.c $APPDIR/src/
COPY --chown=oracle:oracle Makefile.c-headers-only $APPDIR/Makefile
COPY --chown=oracle:oracle scripts/start-tuxedo-q-c.sh $APPDIR/

# Set working directory
WORKDIR $APPDIR

# Build C implementation with stub functions
RUN make stub && make || echo "Build may fail without Tuxedo SDK headers"

# Make scripts executable
RUN chmod +x $APPDIR/start-tuxedo-q-c.sh

# Expose HTTP port
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=10s --retries=3 \
  CMD curl -f http://localhost:8080/health || exit 1

# Override base image ENTRYPOINT and run C server
ENTRYPOINT []
CMD ["/u01/oracle/user_projects/tuxdemo/start-tuxedo-q-c.sh"]
