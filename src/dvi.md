 dist# 3D Viewer
## Makefile targets
- 'make install' установка программы на 1 директорю вверх.
- 'make uninstall' удаление программы.
- 'make dist' архивация src директории.
- 'make dvi' открытыие dvi.md.
- 'make test' запуск тестов, а также make gcov_report запускает программу.
- 'make gcov_report' запуск gcov.
- 'make clean' отчистка директории о мусора.
## User interface
- 'Reset settings' позволяет сбросить настройки камеры и все параметры (пригодится, если установить камеру в некорректном месте).
- Для загрузки модели введите путь в 'Filename:' и нажимите Load (после загрузки будет написано количество вершин и индексов).
- Для отключения пола можно воспользоваться Floor.
- Для отклчения неба можно воспользоваться Sky.
- Для отключения модеи можно воспользоваться Show model.
- Background color меняет цвет фона (если отключен Sky).
- Wireframe включение сетки модели.
- Dotted lines включение пунктирных линий модели.
- Solid model color можно покрыть сплошным цветом модель (ниже приведены настройки).
- В Vertex line можно настроить тип вершин.
- Vertex color может детально настроить цвет вершин.
### Во вкладке Object можно менять
- положение модели по осям X, Y, Z.
- поворот модели по осям X, Y, Z.
- масштабмодели по осям X, Y, Z.
### Во вкладке Camera можно менять
- параллельную проекцию или перспективную.
- Projection size ширина камеры.
- Позицию по х.
- Позицию по у.
- Позицию по z .
- Velocity скорость камеры.
- Ротацию по x,y,z.


