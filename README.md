このリポジトリと使用方法について
===============================

概要
---------------------------

これは、某研修機材のイメージ生成用ツリーです。[Atmark Dist]用のプロダクトディレクトリと
して使用します。



ベース環境
---------------------------

本ツリーがベースとするAtmark Distの環境は以下の通りです。

* [Atmark Dist v20160927](http://armadillo.atmark-techno.com/files/downloads/armadillo-810/source/atmark-dist-20160927.tar.gz)
* [Linuxカーネル v3.4-at20](http://armadillo.atmark-techno.com/files/downloads/armadillo-810/source/linux-3.4-at20.tar.gz)


ビルド方法
---------------------------

1. Atmark Distのビルド環境を用意する。([Armadilo-810製品マニュアル 第11章 ビルド手順]
   を参照してください)。
2. atmark-dist-20160927/vendors/AtmarkTechno/配下で本リポジトリを `git clone` する
3. atmark-dist-20160927/配下で、`make menuconfig` を実行し、menuconfigを開く
  (以下、menuconfigの操作については[Armadilo-810製品マニュアル 第11章 ビルド手順]、
  あるいは[Atmark Dist開発者ガイド]を参照してください)。
4. [Vendor/Product Selection]で、[Vendor]に"AtmarkTechno"、[Products]に
   "training-c"を選択する。
5. [Kernel/Library/Defaults Selection]で、[Default all settings (lose changes)]
   を選択する。
6. [Exit]で、設定を保存した上でmenuconfigを終了する。
7. `vendors/AtmarkTechno/training-c/tools/apply_patch` を実行する
8. atmark-dist-20160927/配下で　`make` を実行する。


参考
---------------------------
* [Atmark Dist開発者ガイド]


[Atmark Dist]: http://armadillo.atmark-techno.com/atmark-dist
[Armadilo-810製品マニュアル 第11章 ビルド手順]: http://manual.atmark-techno.com/armadillo-810/armadillo-810_product_manual_ja-1.6.2/ch11.html#sct.build-dist
[Atmark Dist開発者ガイド]: http://armadillo.atmark-techno.com/files/downloads/dist/atmark-dist_developers_guide_ja-1.0.11.pdf
