(function () {

    "use strict";

    var w3c_include = document.getElementById("w3c-include"),
        style;

    w3c_include.setAttribute("lang", "en");
    w3c_include.setAttribute("dir", "ltr");

    function addDonationProgram() {

        var program_div, script, sponsor;

        program_div = document.createElement("div");
        program_div.setAttribute("class", "w3c-include");
        program_div.setAttribute("id", "w3c-include-validator-donation");

        w3c_include.appendChild(program_div);

        // http://developers.flattr.net/button/
        (function () {
            var s = document.createElement('script'),
                t = document.getElementsByTagName('script')[0];

            s.type = 'text/javascript';
            s.async = true;
            s.src = '//api.flattr.com/js/0.6/load.js?mode=auto';

            t.parentNode.insertBefore(s, t);
        }());

        function chooseSponsor() {

            var sponsors = {},
                rand_no;

            sponsors.HP = {};
            sponsors.HP.name = "HP";
            sponsors.HP.image = '//www.w3.org/QA/Tools/logos_sup/hp_logo.jpg';
            sponsors.HP.text = 'The W3C validators are hosted on server technology donated by HP, and supported by community donations.';
            sponsors.HP.height = 80;
            sponsors.HP.link = 'http://www.hp.com';

            sponsors.MOZ = {};
            sponsors.MOZ.name = "Mozilla Foundation";
            sponsors.MOZ.image = '//www.w3.org/QA/Tools/logos_sup/moz_logo.jpg';
            sponsors.MOZ.text = 'The W3C validators are developed with assistance from the Mozilla Foundation, and supported by community donations.';
            sponsors.MOZ.height = 76;
            sponsors.MOZ.link = 'http://www.mozilla.com';

            sponsors.Donate = {};
            sponsors.Donate.name = "Validators Donation Program";
            sponsors.Donate.image = '//www.w3.org/QA/Tools/I_heart_validator_lg';
            sponsors.Donate.text = 'The W3C validators rely on community support for hosting and development.';
            sponsors.Donate.height = 46;
            sponsors.Donate.link = '//www.w3.org/QA/Tools/Donate';

            rand_no = Math.random();
            rand_no = rand_no * 100;
            rand_no = Math.ceil(rand_no);
            if (rand_no <= 34) {
                return sponsors.HP;
            } else if (rand_no <= 60) {
                return sponsors.MOZ;
             } else {
                return sponsors.Donate;
            }
        }

        sponsor = chooseSponsor();

        program_div.innerHTML =
            '<p>' +
                '<a href="' + sponsor.link + '" title="' + sponsor.name + '" class="w3c-include-validator-donation-img">' +
                    '<img src="' + sponsor.image + '" alt="' + sponsor.name + ' logo" height="' + sponsor.height + '" class="w3c-include-sponsor-img">' +
                '</a>' +
            '</p>' +
            '<p>' +
                '<a href="http://validator.w3.org" title="View W3C-Validator on flattr.com" class="FlattrButton" lang="en">Flattr us!</a>' +
                '<span>' + sponsor.text + '</span>' +
                '<br />' +
                '<span><a href="//www.w3.org/QA/Tools/Donate">Donate</a> and help us build better tools for a better web.</span>' +
            '</p>';

    }

    style = document.createElement("style");
    style.setAttribute("type", "text/css");
    style.textContent = [
        '#w3c-include { display: table; width: 100%; box-sizing: border-box; margin: 1em 0 0 0; padding: 0; }',
        '.w3c-include { display: table-row; padding-bottom: 1em; }',
        '.w3c-include > p { display: table-cell; vertical-align: middle; text-align: center; padding-bottom: 1em; }',
        '.w3c-include > p:first-of-type { padding-right: 1em; width: 20%; padding-left: 2em; }',
        '.w3c-include > p:last-of-type { padding-right: 2em;}',
        '.w3c-include-sponsor-img { margin-left: 1em; }',
        '.FlattrButton { float: right; margin-right: 1em }'
    ].join('\n');

    document.getElementsByTagName("head")[0].appendChild(style);

    addDonationProgram();

}());
