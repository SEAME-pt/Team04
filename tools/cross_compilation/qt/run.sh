#!/bin/bash

set -euo pipefail
set -o noglob

CURRENT_DIR="$(cd "$(dirname "$0")" && pwd)"
TAR_FILE="${CURRENT_DIR}/rasp.tar.gz"
DOCKERFILE="${CURRENT_DIR}/DockerFileRasp"
IMAGE_NAME="raspimage"
CONTAINER_NAME="temp-arm"

function log() {
    echo "[INFO] $1"
}

function error() {
    echo "[ERROR] $1" >&2
    exit 1
}

function build_and_copy_tar() {
    log "Building Docker image '${IMAGE_NAME}' for ARM64..."
    sudo docker buildx build --platform linux/arm64 --load -f "${DOCKERFILE}" -t "${IMAGE_NAME}" .

    log "Creating temporary container '${CONTAINER_NAME}'..."
    sudo docker create --name "${CONTAINER_NAME}" "${IMAGE_NAME}"

    log "Copying rasp.tar.gz from container to '${CURRENT_DIR}'..."
    sudo docker cp "${CONTAINER_NAME}:/build/rasp.tar.gz" "${TAR_FILE}"

    log "Cleaning up temporary container..."
    sudo docker rm -f "${CONTAINER_NAME}" >/dev/null
}

function main() {
    if [ ! -f "${TAR_FILE}" ]; then
        log "File '${TAR_FILE}' does not exist. Starting the build process..."
        if [ ! -f "${DOCKERFILE}" ]; then
            error "Dockerfile '${DOCKERFILE}' not found. Please check the path."
        fi
        build_and_copy_tar
        log "File '${TAR_FILE}' has been successfully created."
    else
        log "File '${TAR_FILE}' already exists. Skipping the build process."
    fi
}

main
