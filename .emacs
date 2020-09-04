
(package-initialize)

(global-linum-mode t)
(setq linum-format "%4d | ")

(setq c-default-style "linux")

(require 'cedet)
(semantic-mode t)

(setq semanticdb-default-save-directory "~/.emacs.d/")

(global-set-key (kbd "C-q") 'semantic-ia-complete-symbol-menu)
(global-set-key (kbd "C-w") 'semantic-ia-fast-jump)

(semantic-add-system-include "/user/include" 'c++-mode)
(semantic-add-system-include "/user/include/c++/" 'c++-mode)
(semantic-add-system-include "/usr/include/c++/7/bits/" 'c++-mode)

;(global-set-key(kbd "TAB") 'self-insert-command)
;(setq default-tab-width 4)
;(setq backward-delete-char-untabify-method nil)
;(defun my-c-mode-hook ()
;  (setq c-basic-offset 4
;	c-indent-level 4
;	c-default-style "bsd"))
;(add-hook 'c-mode-common-hook 'my-c-mode-hook)


