all:
	clang getAllMedicine.c -o getAllMedicine -O2 -I/usr/local/include/json-glib-1.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -pthread -L/usr/local/lib -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -L/usr/local/lib -lsqlite3
	clang increaseMedicine.c -o increaseMedicine -O2 -I/usr/local/include/json-glib-1.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -pthread -L/usr/local/lib -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -L/usr/local/lib -lsqlite3
	clang modifyMedicine.c -o modifyMedicine -O2 -I/usr/local/include/json-glib-1.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -pthread -L/usr/local/lib -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -L/usr/local/lib -lsqlite3
	clang getHistoryByID.c -o getHistoryByID -O2 -I/usr/local/include/json-glib-1.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -pthread -L/usr/local/lib -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -L/usr/local/lib -lsqlite3
	clang getHistoryOverview.c -o getHistoryOverview -O2 -I/usr/local/include/json-glib-1.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -pthread -L/usr/local/lib -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -L/usr/local/lib -lsqlite3
clean:
	rm getAllMedicine increaseMedicine modifyMedicine getHistoryByID getHistoryOverview
