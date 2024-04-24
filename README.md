Реализация алгоритма KMeans++ с использованием параллельного программирования 
1.  Выбирается первый центроид случайным образом из набора данных.
2.  Для каждой точки данных вычисляется расстояние от нее до ближайшего уже выбранного центроида.
3.  Новый центроид выбирается случайным образом с вероятностью, пропорциональной квадратам расстояний до ближайших центроидов.
4.  Шаги 2 и 3 повторяются до тех пор, пока не будут выбраны все k центроидов.
