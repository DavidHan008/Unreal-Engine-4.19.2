#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$BASH_SOURCE")" ; pwd)
TOP_DIR=$(cd "$SCRIPT_DIR/../../.." ; pwd)

# args: package
# returns 0 if installed, 1 if it needs to be installed
PackageIsInstalled()
{
    PackageName=$1

    dpkg-query -W -f='${Status}\n' $PackageName | head -n 1 | awk '{print $3;}' | grep -q '^installed$'
}

# main
set -e

TOP_DIR=$(cd "$SCRIPT_DIR/../../.." ; pwd)
cd "${TOP_DIR}"

if [ -e /etc/os-release ]; then
  source /etc/os-release
  # Ubuntu/Debian/Mint
  if [[ "$ID" == "ubuntu" ]] || [[ "$ID_LIKE" == "ubuntu" ]] || [[ "$ID" == "debian" ]] || [[ "$ID_LIKE" == "debian" ]] || [[ "$ID" == "tanglu" ]] || [[ "$ID_LIKE" == "tanglu" ]]; then
    # Install the necessary dependencies (require clang-3.8 on 16.04, although 3.3 and 3.5 through 3.7 should work too for this release)
     # mono-devel is needed for making the installed build (particularly installing resgen2 tool)

    # Adjust the VERSION_ID for elementary OS since elementary doesn't follow Debian versioning, but is Ubuntu-based.
    if [[ "$ID" == "elementary" ]]; then
      if [[ "$VERSION_ID" == 0.4 ]] || ([[ "$VERSION_ID" > 0.4 ]] && [[ "$VERSION_ID" < 0.5 ]]); then
        VERSION_ID=16.04 # The 0.4 branch is based on 16.04
      elif [[ "$VERSION_ID" == 0.5 ]] || [[ "$VERSION_ID" > 0.5 ]]; then
        VERSION_ID=17 # Assumes that 0.5 won't be 16.04 based
      fi
    fi

    if [ -n "$VERSION_ID" ] && [[ "$VERSION_ID" < 16.04 ]]; then
     DEPS="mono-xbuild \
       mono-dmcs \
       libmono-microsoft-build-tasks-v4.0-4.0-cil \
       libmono-system-data-datasetextensions4.0-cil
       libmono-system-web-extensions4.0-cil
       libmono-system-management4.0-cil
       libmono-system-xml-linq4.0-cil
       libmono-corlib4.0-cil
       libmono-windowsbase4.0-cil
       libmono-system-io-compression4.0-cil
       libmono-system-io-compression-filesystem4.0-cil
       mono-devel
       clang-3.5
       build-essential
       "
    elif [ -n "$VERSION_ID" ] && [[ "$VERSION_ID" == 16.04 ]]; then
     DEPS="mono-xbuild \
       mono-dmcs \
       libmono-microsoft-build-tasks-v4.0-4.0-cil \
       libmono-system-data-datasetextensions4.0-cil
       libmono-system-web-extensions4.0-cil
       libmono-system-management4.0-cil
       libmono-system-xml-linq4.0-cil
       libmono-corlib4.5-cil
       libmono-windowsbase4.0-cil
       libmono-system-io-compression4.0-cil
       libmono-system-io-compression-filesystem4.0-cil
       libmono-system-runtime4.0-cil
       mono-devel
       clang-3.8
       llvm
       build-essential
       "
    elif [ -n "$VERSION_ID" ] && [[ "$VERSION_ID" < 17.10 ]]; then
     DEPS="mono-xbuild \
       mono-dmcs \
       libmono-microsoft-build-tasks-v4.0-4.0-cil \
       libmono-system-data-datasetextensions4.0-cil
       libmono-system-web-extensions4.0-cil
       libmono-system-management4.0-cil
       libmono-system-xml-linq4.0-cil
       libmono-corlib4.5-cil
       libmono-windowsbase4.0-cil
       libmono-system-io-compression4.0-cil
       libmono-system-io-compression-filesystem4.0-cil
       libmono-system-runtime4.0-cil
       mono-devel
       clang-3.9
       llvm
       build-essential
       "
    elif [[ $PRETTY_NAME == *sid ]] || [[ $PRETTY_NAME == *stretch ]]; then
     DEPS="mono-xbuild \
       mono-dmcs \
       libmono-microsoft-build-tasks-v4.0-4.0-cil \
       libmono-system-data-datasetextensions4.0-cil
       libmono-system-web-extensions4.0-cil
       libmono-system-management4.0-cil
       libmono-system-xml-linq4.0-cil
       libmono-corlib4.5-cil
       libmono-windowsbase4.0-cil
       libmono-system-io-compression4.0-cil
       libmono-system-io-compression-filesystem4.0-cil
       libmono-system-runtime4.0-cil
       mono-devel
       clang-3.8
       "
    else # assume the latest Ubuntu, this is going to be a moving target (17.10 as of now)
     DEPS="mono-xbuild \
       mono-dmcs \
       libmono-microsoft-build-tasks-v4.0-4.0-cil \
       libmono-system-data-datasetextensions4.0-cil
       libmono-system-web-extensions4.0-cil
       libmono-system-management4.0-cil
       libmono-system-xml-linq4.0-cil
       libmono-corlib4.5-cil
       libmono-windowsbase4.0-cil
       libmono-system-io-compression4.0-cil
       libmono-system-io-compression-filesystem4.0-cil
       libmono-system-runtime4.0-cil
       mono-devel
       clang-5.0
       lld-5.0
       llvm
       build-essential
       "
    fi

    # these tools are only needed to build third-party software which is prebuilt for Ubuntu.
    if [[ "$ID" != "ubuntu" ]]; then
      DEPS+="libqt4-dev \
             cmake
            "
    fi

    for DEP in $DEPS; do
      if ! PackageIsInstalled $DEP; then
        echo "Attempting installation of missing package: $DEP"
        set -x
        sudo apt-get install -y $DEP
        set +x
      fi
    done
  fi
  
  # openSUSE/SLED/SLES
  if [[ "$ID" == "opensuse" ]] || [[ "$ID_LIKE" == "suse" ]]; then
    # Install all necessary dependencies
    DEPS="mono-core
      mono-devel
      mono-mvc
      libqt4-devel
      dos2unix
      cmake
      "

    for DEP in $DEPS; do
      if ! rpm -q $DEP > /dev/null 2>&1; then
        echo "Attempting installation of missing package: $DEP"
        set -x
        sudo zypper -n install $DEP
        set +x
      fi
    done
  fi

  # Fedora
  if [[ "$ID" == "fedora" ]]; then
    # Install all necessary dependencies
    DEPS="mono-core
      mono-devel
      qt-devel
      dos2unix
      cmake
      clang
      "

    for DEP in $DEPS; do
      if ! rpm -q $DEP > /dev/null 2>&1; then
        echo "Attempting installation of missing package: $DEP"
        set -x
        sudo dnf -y install $DEP
        set +x
      fi
    done
  fi


  # Arch Linux
  if [[ "$ID" == "arch" ]] || [[ "$ID_LIKE" == "arch" ]]; then
    DEPS="clang mono python sdl2 qt4 dos2unix cmake"
    MISSING=false
    for DEP in $DEPS; do
      if ! pacman -Qs $DEP > /dev/null 2>&1; then
        MISSING=true
        break
      fi
    done
    if [ "$MISSING" = true ]; then
      echo "Attempting to install missing packages: $DEPS"
      set -x
      sudo pacman -S --needed --noconfirm $DEPS
      set +x
    fi
  fi
fi

# Provide the hooks for locally building third party libs if needed
echo
pushd Build/BatchFiles/Linux > /dev/null
./BuildThirdParty.sh
popd > /dev/null

echo "Setup successful."
touch Build/OneTimeSetupPerformed
