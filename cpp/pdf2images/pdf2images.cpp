#include <iostream>
#include <string>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>

bool pdf2images(const std::string &pdfFileName, const std::string &imageDir, std::string *err)
{
    std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(pdfFileName));
    if (!doc.get()) {
        if (err != nullptr) *err = "loading error";
        return false;
    }
    if (doc->is_locked()) {
        if (err != nullptr) *err = "encrypted document";
        return false;
    }
    int count = doc->pages();
    for (int i = 0; i < count; ++i) {
        std::unique_ptr<poppler::page> p(doc->create_page(i));
        if (!p.get()) {
            if (err != nullptr) *err = "NULL page";
            return false;
        }
        poppler::page_renderer pr;
        pr.set_render_hint(poppler::page_renderer::antialiasing, true);
        pr.set_render_hint(poppler::page_renderer::text_antialiasing, true);
        poppler::image img = pr.render_page(p.get());
        if (!img.is_valid()) {
            if (err != nullptr) *err = "rendering failed";
            return false;
        }
        if (!img.save(imageDir + "/" + std::to_string(i) + ".png", "png")) {
            if (err != nullptr) *err = "saving to file failed";
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: pdf2images <pdf file path> <output dir>" << std::endl;
        return -1;
    }
    std::string error;
    pdf2images(argv[1], argv[2], &error);
    return 0;
}