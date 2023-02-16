# Dockerfile

FROM fedora:latest

RUN dnf -y update && dnf -y install curl rpmdevtools policycoreutils-devel gcc vim selinux-policy-targeted selinux-policy libselinux-utils

COPY .vimrc $HOME/.
