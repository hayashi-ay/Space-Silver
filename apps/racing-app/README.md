## Racing App
UNIXのシステムコールを用いた簡易的なレーシングアプリです。  

## TODO
- 実行ごとにクライアント側でレース画面が表示されたりされなかったりする
- 初期データがクライアントで受け取る際におかしくなっている

タイミングの問題かもしれない。非同期にsocketから読み込む際にソケットにデータがあるかを確認せずに連続で読み込んでしまっているので、通信の状況によってはデータがあったりなかったりするのかなと考えている。
