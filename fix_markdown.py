import re

def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    # 1. Remove all <a id="..."></a> lines that we added
    content = re.sub(r'<a id="[^"]+"></a>\n', '', content)
    
    # 2. Fix the Chapter headings:
    # Match: ## N. Title <span ...>[Torna all'indice](#indice)</span>
    # and replace with:
    # ## N. Title
    # <div align="right"><em><a href="#indice">Torna all'indice</a></em></div>
    
    pattern = r'^(##\s+\d+\.\s+.*?)\s*<span[^>]*>\[Torna all\'indice\]\(#indice\)</span>'
    
    def repl(m):
        return m.group(1) + '\n<div align="right"><em><a href="#indice">Torna all\'indice</a></em></div>'
        
    content = re.sub(pattern, repl, content, flags=re.MULTILINE)

    # 3. Handle sub-TOC headings:
    # Change "### Indice delle Sottosezioni - Capitolo N"
    # to "### Indice delle Sottosezioni Capitolo N"
    content = re.sub(r'### Indice delle [sS]ottosezioni\s*-\s*Capitolo\s+(\d+)', r'### Indice delle Sottosezioni Capitolo \1', content)
    
    # 4. Also fix the TOC links for sub-TOC to match the new slug (single hyphen)
    # [Sottosezioni](#indice-delle-sottosezioni---capitolo-N) -> [Sottosezioni](#indice-delle-sottosezioni-capitolo-N)
    content = re.sub(r'\[Sottosezioni\]\(#indice-delle-sottosezioni---+capitolo-(\d+)\)', r'[Sottosezioni](#indice-delle-sottosezioni-capitolo-\1)', content)

    # Write back
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)
        
process_file('Appunti/Guida_Completa_LSO_temp.md')
process_file('Appunti/Guida_Completa_LSO.md')
print("Fixes applied.")
