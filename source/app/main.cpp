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
#include <MinMaxAgent.hpp>

int hasarg(char** argv, int argc, const std::string& arg, bool value = false) {
	for(int i = 0; i < argc; ++i) {
		if(arg == argv[i]) {
			if(i > argc -1 && value) return -1;
			return i;
		}
	}
	return -1;
}

bool has_arg(char** argv, int argc, const std::string& arg, int& pos, bool value = false)
{
	for(int i = 0; i < argc; ++i) {
		if(arg == argv[i]) {
			if(i > argc -1 && value) return false;
			pos = i; return true;
		}
	}
	return false;
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
	bool showlabels = true;
	int deparg = hasarg(argv, argc, "-graph-depth", true);
	if(deparg != -1) {
		depth = atoi(argv[deparg+1]);
	}
	int vdeparg = hasarg(argv, argc, "-graph-view", true);
	if(vdeparg != -1) {
		vdepth = atoi(argv[vdeparg+1]);
	}
	int vlabelarg = hasarg(argv, argc, "-graph-silent");
	if(vlabelarg!= -1) {
		showlabels = false;
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

		out << QString::fromStdString(GraphGen::graph(b, depth, vdepth, showlabels));
	}
	else if(has_arg(argv, argc, "-count-from", arc, true)) {
		std::string seq = argv[arc+1];
		size_t n = atoi(seq.substr(0,1).c_str());
		GraphGen::setPrepareImageCallback(graphcb);
		GameBoard b(n, seq.size() > 1 ? seq.substr(1) : std::string((n*n*n)+1, '_'));

		std::cout << b.toString() << std::endl;

		int searchDepth = 2; int searchDepth_arg = 0;
		if(has_arg(argv, argc, "-count-depth", searchDepth_arg, true)) {
			searchDepth = atoi(argv[searchDepth_arg+1]);
		}

		MinMaxAgent agent(T_WHITE, searchDepth-2);
		int states_searched = 0;
		agent.setStateEvaluatedCallback([&](const GameBoard&, const GameBoard&, float, float, float, size_t, char)
		{
			states_searched ++;
		});
		agent.calculateMove(b);

		float stateSize = 1; int num = 57; int tget = num - searchDepth;
		while(num > tget) {
			stateSize = stateSize*(num--);
		}

		std::cout << "States Searched: " << states_searched << " with dlimit: " << searchDepth << " Fraction: " << (states_searched/stateSize) << std::endl;
	}
	else {
		MainWindow mw;

		int sarg = hasarg(argv, argc, "-start-from", true);
		if(sarg != -1) {
			std::string seq = argv[sarg+1];
			size_t n = atoi(seq.substr(0,1).c_str());
			GameBoard b(n, seq.substr(1));
			mw.setStartBoard(b);
		}

		mw.show();

		return app.exec();
	}
	
	return 0;
}
