/** This is free and unencumbered software released into the public domain. **/

#include <QApplication>
#include <QFileDialog>
#include <stdlib.h>
#include <string.h>
#include "plugin.h"


extern "C" API_PUBLIC qt_file_list
  qt_file_select(qt_file_cfg *pcfg, int argc, char **argv)
{
  qt_file_cfg cfg;
  qt_file_list list;
  list.size = 0;
  list.list = NULL;

  /* use default settings */
  if (!pcfg) {
    cfg.path = NULL;
    cfg.title = NULL;
    cfg.filter = NULL;
    cfg.bnative = true;
    cfg.bdir = false;
    cfg.bsave = false;
    cfg.blist = false;
    cfg.bmulti = false;
    pcfg = &cfg;
  }

  QApplication app(argc, argv);
  QFileDialog dlg(NULL, pcfg->title);

  /* not needed */
  dlg.setOption(QFileDialog::DontConfirmOverwrite, true);

  /* set directory */
  if (pcfg->path) {
    dlg.setDirectory(pcfg->path);
  }

  /* use native dialog? */
  dlg.setOption(QFileDialog::DontUseNativeDialog, !pcfg->bnative);

  /* open or save */
  dlg.setAcceptMode(pcfg->bsave ? QFileDialog::AcceptSave : QFileDialog::AcceptOpen);

  /* file mode */
  if (pcfg->bdir) {
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly);
  } else {
    dlg.setFileMode(pcfg->bmulti ? QFileDialog::ExistingFiles : QFileDialog::ExistingFile);
  }

  /* view mode */
  dlg.setViewMode(pcfg->blist ? QFileDialog::List : QFileDialog::Detail);

  /* filters */
  if (pcfg->filter) {
    QString qstr = pcfg->filter;
    QStringList qlist = qstr.split(',', Qt::SkipEmptyParts);
    qlist << "All files (*)";
    dlg.setNameFilters(qlist);
  }

  if (dlg.exec() && !dlg.selectedFiles().isEmpty()) {
    list.size = dlg.selectedFiles().size();
    list.list = reinterpret_cast<char **>(malloc(list.size * sizeof(char *)));
    size_t idx = 0;

    for (const QString &s : dlg.selectedFiles()) {
      list.list[idx] = strdup(s.toUtf8().constData());
      idx++;
    }
  }

  app.quit();

  return list;
}
