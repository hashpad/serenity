@GUI::Frame {
    fill_with_background_color: true
    layout: @GUI::VerticalBoxLayout {
        margins: [8]
    }

    @GUI::Frame {
        layout: @GUI::HorizontalBoxLayout {}
        name: "preview_frame"
        fixed_width: 136
        fixed_height: 136
    }

    @GUI::GroupBox {
        title: "Highlight color"
        fixed_height: 80
        layout: @GUI::VerticalBoxLayout {
            margins: [6]
            spacing: 2
        }

        @GUI::Widget {}

        @GUI::ColorInput {
            name: "highlight_color_input"
            has_alpha_channel: false
        }

        @GUI::Widget {}
    }

    @GUI::GroupBox {
        title: "Highlight opacity"
        fixed_height: 80
        layout: @GUI::VerticalBoxLayout {
            margins: [6]
            spacing: 2
        }

        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout {
                margins: [8]
                spacing: 8
            }

            @GUI::Label {
                autosize: true
                text: "Low"
            }

            @GUI::Slider {
                name: "highlight_opacity_slider"
                orientation: "Horizontal"
                max: 230
                min: 30
                value: 110
            }

            @GUI::Label {
                autosize: true
                text: "High"
            }
        }
    }

    @GUI::GroupBox {
        title: "Highlight size"
        fixed_height: 80
        layout: @GUI::VerticalBoxLayout {
            margins: [6]
            spacing: 2
        }

        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout {
                margins: [8]
                spacing: 8
            }

            @GUI::Label {
                autosize: true
                text: "Small"
            }

            @GUI::Slider {
                name: "highlight_radius_slider"
                orientation: "Horizontal"
                max: 60
                min: 20
                value: 25
            }

            @GUI::Label {
                autosize: true
                text: "Large"
            }
        }
    }
}
