#include "SurgeStyle.hpp"
#include "rack.hpp"
#include "svg.hpp"

int SurgeStyle::fid = -1;
int SurgeStyle::fidcond = -1;

/*
** These are purposefully not exposed
*/
namespace SurgeInternal {

typedef std::shared_ptr<rack::Svg> svg_t;
    
static svg_t surgeLogoBlue = nullptr, surgeLogoWhite = nullptr;
static svg_t getSurgeLogo(bool whiteVersion) {
    if (surgeLogoBlue == nullptr) {
        surgeLogoBlue = rack::APP->window->loadSvg(
            rack::asset::plugin(pluginInstance, "res/SurgeLogoOnlyBlue.svg"));
        surgeLogoWhite = rack::APP->window->loadSvg(
            rack::asset::plugin(pluginInstance, "res/SurgeLogoOnlyWhite.svg"));
    }
    if (whiteVersion)
        return surgeLogoWhite;
    return surgeLogoBlue;
}
} // namespace SurgeInternal

std::unordered_set<SurgeStyle::StyleListener *> SurgeStyle::listeners;

void SurgeStyle::drawBlueIORect(NVGcontext *vg, float x0, float y0, float w,
                                float h, int direction) {
    nvgBeginPath(vg);
    NVGpaint vGradient;
    vGradient =
        nvgLinearGradient(vg, x0, y0, x0, y0 + h,
                          ioRegionBackgroundGradientStart(),
                          ioRegionBackgroundGradientEnd());

    nvgRoundedRect(vg, x0, y0, w, h, 5);
    nvgFillPaint(vg, vGradient);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x0, y0 + 1, w, h - 1, 5);
    nvgStrokeColor(vg, ioRegionBorderHighlight() );
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x0, y0, w, h, 5);
    nvgStrokeColor(vg, ioRegionBorder() );
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);
    nvgBeginPath(vg);
}

void SurgeStyle::drawTextBGRect(NVGcontext *vg, float x0, float y0, float w,
                                float h) {
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x0, y0, w, h, 5);
    NVGpaint gradient =
        nvgLinearGradient(vg, x0, y0, x0, y0 + h, SurgeStyle::textBGGradientStart(),
                          SurgeStyle::textBGGradientEnd());
    nvgFillPaint(vg, gradient);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x0, y0, w, h - 1, 5);
    nvgStrokeColor(vg, SurgeStyle::textBGBorderHighlight());
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x0, y0, w, h, 5);
    nvgStrokeColor(vg, SurgeStyle::textBGBorder());
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);
}

void SurgeStyle::drawPanelBackground(NVGcontext *vg, float w, float h,
                                     std::string displayName, bool narrowMode) {
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, w, h);
    nvgFillColor(vg, SurgeStyle::panelBackground() );
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, 0, h );
    nvgLineTo(vg, 0, 0 );
    nvgLineTo(vg, w, 0 );
    nvgLineTo(vg, w, h);
    nvgStrokeColor(vg, SurgeStyle::panelBackgroundOutline());
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgRect(vg, 0, orangeLine, w, h - orangeLine);
    nvgFillColor(vg, SurgeStyle::panelFooter() );
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, w, orangeLine);
    nvgLineTo(vg, w, h);
    nvgLineTo(vg, 0, h );
    nvgLineTo(vg, 0, orangeLine );
    nvgStrokeColor(vg, SurgeStyle::panelFooterOutline() );
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, 0, orangeLine);
    nvgLineTo(vg, w, orangeLine);
    nvgStrokeColor(vg, SurgeStyle::panelFooterSeparator() );
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    auto logoSvg = SurgeInternal::getSurgeLogo(false);
    float logoX0 = w / 2;
    
    if (narrowMode)
    {
        logoX0 = 2;
    }
    
    float logoWidth = 10;
    if (logoSvg && logoSvg->handle) {
        // We want the logo to be screw width - 4  high
        float scaleFactor = 1.0 * (SCREW_WIDTH - 4) / logoSvg->handle->height;
        float x0 = logoX0 - (narrowMode ? 0 : logoSvg->handle->width * scaleFactor / 2 );
        logoWidth = logoSvg->handle->width * scaleFactor;
        nvgSave(vg);
        nvgTranslate(vg, x0, 2);
        nvgScale(vg, scaleFactor, scaleFactor);
        // FIXME - we want to replace the color in this logo with the panelTitle color
        rack::svgDraw(vg, logoSvg->handle);
        nvgRestore(vg);
    }
        
    if( ! narrowMode )
    {
        nvgBeginPath(vg);
        nvgTextAlign(vg, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
        nvgFontFaceId(vg, fontId(vg));
        nvgFontSize(vg, 14);
        nvgFillColor(vg, SurgeStyle::panelTitle() );
        nvgText(vg, logoX0 - logoWidth / 2 - 3, 0, "Surge", NULL);
    }
    
    nvgBeginPath(vg);
    nvgTextAlign(vg, (narrowMode ? NVG_ALIGN_RIGHT | NVG_ALIGN_TOP : NVG_ALIGN_LEFT | NVG_ALIGN_TOP ) );
    nvgFontFaceId(vg, fontId(vg));
    nvgFontSize(vg, 14);
    nvgFillColor(vg, SurgeStyle::panelTitle() );
    nvgText(vg, (narrowMode ? w - 2 : logoX0 + logoWidth / 2 + 3 ), 0, displayName.c_str(), NULL);
}
