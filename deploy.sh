echo "Inicializando procedimento de deploy"
rm -r ./_site/
jekyll build
git checkout master
echo "Atualizando Projeto Local"
git pull
echo "Adicionando novo conteúdo no site"
cp -r _site/* . && rm -rf _site/