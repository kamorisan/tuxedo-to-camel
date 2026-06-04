# Oracle Tuxedo Base Image with SDK
# Adds SDK components (headers + build tools) to oracle/tuxedo runtime image

FROM oracle/tuxedo:22.1.1.0.0

USER root

# Install development tools
RUN yum -y install gcc make libmicrohttpd-devel && yum clean all

# Copy Tuxedo installer
COPY tuxedo221100_64_linux_01_x86.zip /tmp/

# Extract and install SDK
RUN cd /tmp && \
    unzip -q tuxedo221100_64_linux_01_x86.zip && \
    cd Disk1/install && \
    # Silent installation with response file
    ./runInstaller.sh -silent \
        -responseFile /tmp/response.txt \
        -invPtrLoc /tmp/oraInst.loc && \
    # Cleanup
    rm -rf /tmp/tuxedo221100_64_linux_01_x86.zip /tmp/Disk1

# Copy response files for installation
COPY response.txt /tmp/response.txt
COPY oraInst.loc /tmp/oraInst.loc

USER oracle

# Set environment
ENV TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
ENV PATH=$TUXDIR/bin:$PATH
ENV LD_LIBRARY_PATH=$TUXDIR/lib:$LD_LIBRARY_PATH

# Verify installation
RUN ls -la $TUXDIR/include/atmi.h && \
    ls -la $TUXDIR/include/tmqueue.h && \
    which tmloadcf && \
    which tmboot

CMD ["/bin/bash"]
