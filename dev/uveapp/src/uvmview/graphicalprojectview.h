 /*
    UVE is a free open source software able to automatically generate
    UVM/SystemVerilog testbenches
    Copyright (C) 2012 HES-SO

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GRAPHICALPROJECTVIEW_H
#define GRAPHICALPROJECTVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "uvmproject.h"
#include "uvmcomponentview.h"
#include "uvmtopview.h"
#include "connect.h"

/** Prints a graphical view of the project
  Displays each element of the project as a box, linked with each other by their ports/connections.
  */
class GraphicalProjectView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicalProjectView(MainWindow *mainwindow);
    ~GraphicalProjectView();

    //! Simple getter
    UvmProject* getProject () const;

    void changeActiveComponent(QString pathOfNewlyActiveComponent);
    void noActiveComponent();

    QPixmap getPixmap();

signals:

public slots:

    /** Read and display this project
      The old view is removed, and this project parsed.
      */
    void setProject (UvmProject *project);

    //! Refresh the current project's view
    void refresh ();

    void refreshPlacement();

private:

    UvmTopView* topView;

    //Create the right view from the specified component
    UvmComponentView* createUVMView(UvmComponentView* parentView, UvmComponent* comp);

    //Route the connection (I think)
    void routeUVMView (UvmTopView *topView);

    //Our project
    UvmProject *_project;
    //Our scene (graphic view)
    QGraphicsScene *_scene;

    //    RoutingBoard* board;
    //The main window (for file opening)
    MainWindow *mainwindow;
};

#endif // GRAPHICALPROJECTVIEW_H
