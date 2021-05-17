# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.177.0/containers/codespaces-linux/.devcontainer/base.Dockerfile

FROM mcr.microsoft.com/vscode/devcontainers/universal:1-focal
RUN cd /home/codespace \
    && git clone https://github.com/emscripten-core/emsdk \
    && cd emsdk \ 
    && ./emsdk install latest \
    && ./emsdk activate latest \ 
    && echo 'source /home/codespace/emsdk/emsdk_env.sh' >> /home/codespace/.bashrc \
    && echo 'source /home/codespace/emsdk/emsdk_env.sh' >> /home/codespace/.zshrc
# ** [Optional] Uncomment this section to install additional packages. **
# USER root
#
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>
#
# USER codespace
