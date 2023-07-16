# Espressif's SDK for Matter Docker Image

This is a Docker image for the [Espressif's SDK for Matter (ESP-MATTER)](https://github.com/espressif/esp-matter). It is intended for building applications of ESP-IDF that uses Espressif's SDK for Matter, when doing automated builds.

This image contains a copy of the Espressif's SDK for Matter, a copy of ESP-IDF and the required tools for Matter to build ESP-IDF projects that use Espressif's SDK for Matter.

## Basic Usage

Build a project located in the current directory using `idf.py build` command:

```bash
docker run --rm -v $PWD:/project -w /project espressif/esp-matter:latest idf.py build
```
## Building custom images

The Dockerfile in Espressif's SDK for Matter repository provides several build arguments which can be used to customize the Docker image:

These are the different build arguments that can be used:
- ``ESP_MATTER_CLONE_URL``: URL of the repository to clone Espressif's SDK for Matter. Can be set to a custom URL when working with a fork of Espressif's SDK for Matter. Default is ``https://github.com/espressif/esp-matter.git``.
- ``ESP_MATTER_CLONE_BRANCH_OR_TAG``: Name of a git branch or tag use when cloning Espressif's SDK for Matter. This value is passed to ``git clone`` command using the ``--branch`` argument. Default is ``main``.
- ``ESP_MATTER_CHECKOUT_REF``: If this argument is set to a non-empty value, ``git checkout $ESP_MATTER_CHECKOUT_REF`` command will be performed after cloning. This argument can be set to the SHA of the specific commit to check out, for example if some specific commit on a release branch is desired.

You can also use build arguments to control the ESP-IDF
- ``IDF_CLONE_URL``: URL of the repository to clone ESP-IDF from. Can be set to a custom URL when working with a fork of ESP-IDF. Default is ``https://github.com/espressif/esp-idf.git``.
- ``IDF_CLONE_BRANCH_OR_TAG``: Name of a git branch or tag use when cloning ESP-IDF. This value is passed to ``git clone`` command using the ``--branch`` argument. Default is ``v4.4.3``.
- ``IDF_CHECKOUT_REF``: If this argument is set to a non-empty value, ``git checkout $IDF_CHECKOUT_REF`` command will be performed after cloning. This argument can be set to the SHA of the specific commit to check out, for example if some specific commit on a release branch is desired.
- ``IDF_CLONE_SHALLOW``: If this argument is set to a non-empty value, ``--depth=1 --shallow-submodules`` arguments will be used when performing ``git clone``. This significantly reduces the amount of data downloaded and the size of the resulting Docker image. However, if switching to a different branch in such a "shallow" repository is necessary, an additional ``git fetch origin <branch>`` command must be executed first.
