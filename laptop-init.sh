#!/usr/bin/env bash

echo "Installing git..."
sudo apt install git-all


# ====== SPOTIFY ======
echo "Installing Spotify..."
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 931FF8E79F0876134EDDBDCCA87FF9DF48BF1C90
echo deb http://repository.spotify.com stable non-free | sudo tee /etc/apt/sources.list.d/spotify.list
sudo apt-get update
sudo apt-get install spotify-client
# ======================

# ===== SSH KEYS =====
echo "Generating ssh keys..."
echo "$HOME/.ssh/github" | ssh-keygen
# ======================

# ==== NVM ====
echo "Installing nvm..."
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash



# ==== WORK ====
# gcloud
export CLOUD_SDK_REPO="cloud-sdk-$(lsb_release -c -s)"
echo "deb http://packages.cloud.google.com/apt $CLOUD_SDK_REPO main" | sudo tee -a /etc/apt/sources.list.d/google-cloud-sdk.list
curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
sudo apt-get update && sudo apt-get install google-cloud-sdk

echo "Installing GiftWizard stuff..."
mkdir ~/GiftWizard 
echo "alias ssh-giftwizard='gcloud compute ssh giftwizard-main'"
echo "alias ssh-giftwizard='gcloud compute ssh giftwizard-main'"




source ~/.bashrc

echo "please download chrome"
echo "please download lastpass and login"
echo "please download webstorm"
echo "please download postman"

echo "Github rsa key: $(cat ~/.ssh/github.pub)"



