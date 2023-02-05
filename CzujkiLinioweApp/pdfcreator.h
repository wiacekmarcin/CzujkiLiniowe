#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#ifndef NOPDF

#ifdef __cplusplus
extern "C" {
#endif
int createPdf();

#ifdef __cplusplus
}
#endif

#else
int createPdf();

#endif

#endif // PDFCREATOR_H
