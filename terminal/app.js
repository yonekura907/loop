var express = require('express');

var app = express();

//静的ファイルの公開
app.use(express.static('public'));

//ルーティング設定
app.use('/', require('./routes/index.js'));

//サーバーをポート3000で起動
app.listen(3000);

//アプリケーション開始ログ
console.log('Server running at http://localhost:3000');
