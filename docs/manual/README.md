# Curiosity Trainer Manual (LaTeX)

`curiosity-trainer-manual.tex` is the unified manual, written with the KOMA-Script `scrbook` class:

* **Part I, Development Environment**: installing VS Code, the MPLAB extensions and XC16, with the
  reference configuration (from `../dev-environment`).
* **Part II, Programming Reference**: the content of `../Curiosity Nano Programming Reference.docx`.

## Building

Needs a TeX distribution with KOMA-Script (TeX Live or MiKTeX with the usual packages: `tcolorbox`,
`listings`, `booktabs`, `tabularx`, `longtable`, `enumitem`, `microtype`, `hyperref`). From this folder:

```sh
latexmk -pdf curiosity-trainer-manual.tex
# or run pdflatex (or lualatex) twice, for the table of contents and the references
```

It also builds on Overleaf: upload the `.tex` file and the `figures` folder.

## Regenerating

The `.tex` file is generated: after editing the Word reference or refreshing the environment snapshot,
run `scripts/regenerate.sh`. Edits made directly in the `.tex` file are overwritten by the script.

| Script | Role |
|---|---|
| `scripts/docx2tex.py` | Converts the Word programming reference into LaTeX (Part II) |
| `scripts/build_manual.py` | Writes Part I, the preamble and the title page, and assembles the manual |
| `scripts/check_tex.py` | Checks braces, environments, references, figures and special characters |
| `scripts/regenerate.sh` | Runs the three scripts above |
