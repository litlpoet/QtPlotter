;;; .dir-locals.el --- QtPlotter project local variables
;;; Commentary:
;;; Code:
((nil
  .
  ((projectile-project-compilation-cmd
    . "make -j 4 -C \"~/VersionControl/Modules/QtPlotter/build\"")
   (eval
    . (setq-local
       projectile-project-run-cmd
       (expand-file-name "~/VersionControl/Modules/QtPlotter/build/Plotter")))
   )
  )
 )
;;; .dir-locals.el ends here
