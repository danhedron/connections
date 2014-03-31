#include <iostream>
#include <QApplication>
#include "MainWindow.hpp"
#include <GraphGen.hpp>
#include <GameBoard.hpp>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include "GameBoardWidget.hpp"

int hasarg(char** argv, int argc, const std::string& arg, bool value = false) {
	for(int i = 0; i < argc; ++i) {
		if(arg == argv[i]) {
			if(i > argc -1 && value) return -1;
			return i;
		}
	}
	return -1;
}

auto graphcb = [&](const GameBoard& b, const std::string& fname) {
	QFile imfile("graphs/" + QString::fromStdString(fname) + ".png");
	if(!imfile.exists()) {
		QImage image(100, 100, QImage::Format_RGB888);
		QPainter imp(&image);

		GameBoardWidget::paintBoard(&b, imp);

		image.save(imfile.fileName());

		std::cout << "Wrote image " << imfile.fileName().toStdString() << std::endl;
	}
};

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	unsigned int depth = 2, vdepth = 2;
	int deparg = hasarg(argv, argc, "-graph-depth", true);
	if(deparg != -1) {
		depth = atoi(argv[deparg+1]);
	}
	int vdeparg = hasarg(argv, argc, "-graph-view", true);
	if(vdeparg != -1) {
		vdepth = atoi(argv[vdeparg+1]);
	}

	int arc = hasarg(argv, argc, "-graph-from", true);
	if(arc != -1) {
		std::string seq = argv[arc+1];
		size_t n = atoi(seq.substr(0,1).c_str());
		GraphGen::setPrepareImageCallback(graphcb);
		GameBoard b(n, seq.substr(1));

		std::string pathname = "graphs/" + seq.substr(0,1) + ".gv";
		QFile f(pathname.c_str());
		f.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&f);

		std::string gfile(pathname);
		QString fdir = QFileInfo(gfile.c_str()).dir().path();

		GraphGen::setPrepareImageCallback([&](const GameBoard& b, const std::string& fname) {
			QFile imfile(fdir + "/" + QString::fromStdString(fname) + ".png");
			if(!imfile.exists()) {
				QImage image(100, 100, QImage::Format_RGB888);
				QPainter imp(&image);

				GameBoardWidget::paintBoard(&b, imp);

				image.save(imfile.fileName());

				std::cout << "Wrote image " << imfile.fileName().toStdString() << std::endl;
			}
		});

		out << QString::fromStdString(GraphGen::graph(b, depth, vdepth));
	}
	else {
		MainWindow mw;

		mw.show();

		return app.exec();
	}
	
	return 0;
}
