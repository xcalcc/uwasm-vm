(module
  (type (;0;) (func (param i32 i32 i32) (result i32)))
  (type (;1;) (func (param i32 i32) (result i32)))
  (type (;2;) (func))
  (type (;3;) (func (param i32) (result i32)))
  (type (;4;) (func (param i32 i32)))
  (type (;5;) (func (param i32 i32 i32 i32) (result i32)))
  (type (;6;) (func (param i32 i32 i32 i32 i32) (result i32)))
  (type (;7;) (func (param i32)))
  (type (;8;) (func (param i32 i32 i32)))
  (func $.memcmp (type 0) (param i32 i32 i32) (result i32)
    (local i32 i32 i32 i32 i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              local.get 0
              i32.const 7
              i32.and
              local.tee 3
              local.get 1
              i32.const 7
              i32.and
              i32.eq
              if  ;; label = @6
                i32.const 8
                local.get 3
                i32.sub
                local.set 4
                i32.const 0
                local.set 3
                block  ;; label = @7
                  loop  ;; label = @8
                    local.get 0
                    local.get 3
                    i32.add
                    local.set 5
                    local.get 1
                    local.get 3
                    i32.add
                    local.set 6
                    local.get 3
                    local.get 4
                    i32.ge_u
                    br_if 1 (;@7;)
                    local.get 5
                    i32.load8_u
                    local.get 6
                    i32.load8_u
                    i32.ne
                    br_if 3 (;@5;)
                    local.get 3
                    i32.const 1
                    i32.add
                    local.set 3
                    br 0 (;@8;)
                  end
                  unreachable
                end
                local.get 2
                local.get 3
                i32.sub
                local.set 2
                i32.const 0
                local.set 4
                block  ;; label = @7
                  loop  ;; label = @8
                    local.get 2
                    i32.const 4
                    i32.lt_u
                    br_if 1 (;@7;)
                    local.get 5
                    local.get 4
                    i32.add
                    i32.load
                    local.get 6
                    local.get 4
                    i32.add
                    i32.load
                    i32.ne
                    br_if 5 (;@3;)
                    local.get 4
                    i32.const 4
                    i32.add
                    local.set 4
                    local.get 2
                    i32.const -4
                    i32.add
                    local.set 2
                    br 0 (;@8;)
                  end
                  unreachable
                end
                local.get 1
                local.get 3
                i32.add
                local.get 4
                i32.add
                local.set 1
                local.get 0
                local.get 3
                i32.add
                local.get 4
                i32.add
                local.set 0
              end
              i32.const 1
              local.get 2
              i32.sub
              local.set 3
              block  ;; label = @6
                block  ;; label = @7
                  loop  ;; label = @8
                    local.get 3
                    local.tee 4
                    i32.const 1
                    i32.eq
                    br_if 1 (;@7;)
                    local.get 4
                    i32.const 1
                    i32.add
                    local.set 3
                    local.get 1
                    i32.load8_u
                    local.set 2
                    local.get 0
                    i32.load8_u
                    local.set 7
                    local.get 1
                    i32.const 1
                    i32.add
                    local.tee 6
                    local.set 1
                    local.get 0
                    i32.const 1
                    i32.add
                    local.tee 5
                    local.set 0
                    local.get 7
                    local.get 2
                    i32.eq
                    br_if 0 (;@8;)
                    br 2 (;@6;)
                  end
                  unreachable
                end
                local.get 1
                local.set 6
                local.get 0
                local.set 5
              end
              i32.const 0
              local.get 4
              i32.sub
              i32.eqz
              br_if 1 (;@4;)
              br 2 (;@3;)
            end
            local.get 0
            local.get 3
            i32.add
            i32.const 1
            i32.add
            local.set 5
            local.get 1
            local.get 3
            i32.add
            i32.const 1
            i32.add
            local.set 6
            local.get 2
            local.get 3
            i32.sub
            br_if 1 (;@3;)
          end
          i32.const 0
          return
        end
        local.get 5
        i32.load8_u
        local.get 6
        i32.load8_u
        i32.sub
      end
    end)
  (func $.memcpy (type 0) (param i32 i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        local.get 2
        i32.const 16
        i32.add
        local.set 12
        local.get 2
        i32.const -1
        i32.xor
        local.set 6
        local.get 2
        i32.const 15
        i32.add
        local.set 7
        i32.const 0
        local.set 4
        i32.const 0
        local.get 2
        i32.sub
        local.set 8
        local.get 2
        i32.const 14
        i32.add
        local.set 14
        i32.const 1
        local.get 2
        i32.sub
        local.set 10
        local.get 2
        i32.const 13
        i32.add
        local.set 15
        i32.const 2
        local.get 2
        i32.sub
        local.set 9
        local.get 0
        local.set 13
        block  ;; label = @3
          loop  ;; label = @4
            local.get 0
            local.get 4
            i32.add
            local.set 5
            local.get 2
            local.get 4
            i32.eq
            local.get 1
            local.get 4
            i32.add
            local.tee 3
            i32.const 3
            i32.and
            i32.eqz
            i32.or
            br_if 1 (;@3;)
            local.get 5
            local.get 3
            i32.load8_u
            i32.store8
            local.get 12
            i32.const -1
            i32.add
            local.set 12
            local.get 6
            i32.const 1
            i32.add
            local.set 6
            local.get 7
            i32.const -1
            i32.add
            local.set 7
            local.get 8
            i32.const 1
            i32.add
            local.set 8
            local.get 14
            i32.const -1
            i32.add
            local.set 14
            local.get 10
            i32.const 1
            i32.add
            local.set 10
            local.get 15
            i32.const -1
            i32.add
            local.set 15
            local.get 9
            i32.const 1
            i32.add
            local.set 9
            local.get 4
            i32.const 1
            i32.add
            local.set 4
            local.get 13
            i32.const 1
            i32.add
            local.set 13
            br 0 (;@4;)
          end
          unreachable
        end
        local.get 2
        local.get 4
        i32.sub
        local.set 13
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 5
                i32.const 3
                i32.and
                if  ;; label = @7
                  block  ;; label = @8
                    local.get 13
                    i32.const 32
                    i32.lt_u
                    br_if 0 (;@8;)
                    local.get 5
                    i32.const 3
                    i32.and
                    local.tee 6
                    i32.const 1
                    i32.eq
                    br_if 2 (;@6;)
                    local.get 6
                    i32.const 2
                    i32.eq
                    br_if 3 (;@5;)
                    local.get 6
                    i32.const 3
                    i32.ne
                    br_if 0 (;@8;)
                    local.get 5
                    local.get 1
                    local.get 4
                    i32.add
                    local.tee 15
                    i32.load
                    local.tee 9
                    i32.store8
                    local.get 2
                    local.get 4
                    i32.sub
                    i32.const -1
                    i32.add
                    local.set 10
                    local.get 0
                    local.get 8
                    i32.const -19
                    local.get 8
                    i32.const -19
                    i32.gt_u
                    select
                    local.get 7
                    i32.add
                    i32.const -16
                    i32.and
                    i32.add
                    local.get 4
                    i32.add
                    i32.const 1
                    i32.add
                    local.set 11
                    i32.const 0
                    local.set 3
                    block  ;; label = @9
                      loop  ;; label = @10
                        local.get 10
                        i32.const 19
                        i32.lt_u
                        br_if 1 (;@9;)
                        local.get 5
                        local.get 3
                        i32.add
                        local.tee 6
                        i32.const 1
                        i32.add
                        local.get 15
                        local.get 3
                        i32.add
                        local.tee 12
                        i32.const 4
                        i32.add
                        i32.load
                        local.tee 14
                        i32.const 24
                        i32.shl
                        local.get 9
                        i32.const 8
                        i32.shr_u
                        i32.or
                        i32.store
                        local.get 6
                        i32.const 5
                        i32.add
                        local.get 12
                        i32.const 8
                        i32.add
                        i32.load
                        local.tee 9
                        i32.const 24
                        i32.shl
                        local.get 14
                        i32.const 8
                        i32.shr_u
                        i32.or
                        i32.store
                        local.get 6
                        i32.const 9
                        i32.add
                        local.get 12
                        i32.const 12
                        i32.add
                        i32.load
                        local.tee 14
                        i32.const 24
                        i32.shl
                        local.get 9
                        i32.const 8
                        i32.shr_u
                        i32.or
                        i32.store
                        local.get 6
                        i32.const 13
                        i32.add
                        local.get 12
                        i32.const 16
                        i32.add
                        i32.load
                        local.tee 9
                        i32.const 24
                        i32.shl
                        local.get 14
                        i32.const 8
                        i32.shr_u
                        i32.or
                        i32.store
                        local.get 3
                        i32.const 16
                        i32.add
                        local.set 3
                        local.get 10
                        i32.const -16
                        i32.add
                        local.set 10
                        br 0 (;@10;)
                      end
                      unreachable
                    end
                    local.get 1
                    local.get 8
                    i32.const -19
                    local.get 8
                    i32.const -19
                    i32.gt_u
                    select
                    local.get 7
                    i32.add
                    local.tee 5
                    i32.const -16
                    i32.and
                    i32.add
                    local.get 4
                    i32.add
                    i32.const 1
                    i32.add
                    local.set 3
                    local.get 5
                    i32.const -1
                    i32.xor
                    i32.const 15
                    i32.or
                    local.get 2
                    i32.add
                    local.get 4
                    i32.sub
                    local.set 13
                    br 4 (;@4;)
                  end
                  local.get 5
                  local.set 11
                  br 3 (;@4;)
                end
                local.get 1
                local.get 4
                i32.add
                local.set 10
                local.get 0
                local.get 4
                i32.add
                local.set 9
                local.get 0
                local.get 6
                i32.const -16
                local.get 6
                i32.const -16
                i32.gt_u
                select
                local.get 12
                i32.add
                i32.const -16
                i32.and
                i32.add
                local.get 4
                i32.add
                local.set 7
                i32.const 0
                local.set 3
                block  ;; label = @7
                  loop  ;; label = @8
                    local.get 13
                    i32.const 16
                    i32.lt_u
                    br_if 1 (;@7;)
                    local.get 9
                    local.get 3
                    i32.add
                    local.tee 5
                    local.get 10
                    local.get 3
                    i32.add
                    local.tee 8
                    i32.load
                    i32.store
                    local.get 5
                    i32.const 4
                    i32.add
                    local.get 8
                    i32.const 4
                    i32.add
                    i32.load
                    i32.store
                    local.get 5
                    i32.const 8
                    i32.add
                    local.get 8
                    i32.const 8
                    i32.add
                    i32.load
                    i32.store
                    local.get 5
                    i32.const 12
                    i32.add
                    local.get 8
                    i32.const 12
                    i32.add
                    i32.load
                    i32.store
                    local.get 3
                    i32.const 16
                    i32.add
                    local.set 3
                    local.get 13
                    i32.const -16
                    i32.add
                    local.set 13
                    br 0 (;@8;)
                  end
                  unreachable
                end
                block (result i32)  ;; label = @7
                  local.get 1
                  local.get 6
                  i32.const -16
                  local.get 6
                  i32.const -16
                  i32.gt_u
                  select
                  local.get 12
                  i32.add
                  i32.const -16
                  i32.and
                  local.tee 5
                  i32.add
                  local.tee 6
                  local.get 4
                  i32.add
                  local.get 2
                  local.get 5
                  i32.sub
                  local.get 4
                  i32.sub
                  local.tee 3
                  i32.const 8
                  i32.and
                  i32.eqz
                  br_if 0 (;@7;)
                  drop
                  local.get 0
                  local.get 5
                  i32.add
                  local.get 4
                  i32.add
                  local.tee 5
                  local.get 6
                  local.get 4
                  i32.add
                  local.tee 4
                  i64.load align=4
                  i64.store align=4
                  local.get 5
                  i32.const 8
                  i32.add
                  local.set 7
                  local.get 4
                  i32.const 8
                  i32.add
                end
                local.set 4
                local.get 3
                i32.const 4
                i32.and
                if  ;; label = @7
                  local.get 7
                  local.get 4
                  i32.load
                  i32.store
                  local.get 4
                  i32.const 4
                  i32.add
                  local.set 4
                  local.get 7
                  i32.const 4
                  i32.add
                  local.set 7
                end
                local.get 3
                i32.const 2
                i32.and
                if  ;; label = @7
                  local.get 7
                  local.get 4
                  i32.load16_u align=1
                  i32.store16 align=1
                  local.get 7
                  i32.const 2
                  i32.add
                  local.set 7
                  local.get 4
                  i32.const 2
                  i32.add
                  local.set 4
                end
                local.get 3
                i32.const 1
                i32.and
                i32.eqz
                br_if 3 (;@3;)
                local.get 7
                local.get 4
                i32.load8_u
                i32.store8
                local.get 0
                return
              end
              local.get 5
              local.get 1
              local.get 4
              i32.add
              local.tee 14
              i32.load
              local.tee 10
              i32.store8
              local.get 5
              i32.const 1
              i32.add
              local.get 14
              i32.const 1
              i32.add
              i32.load8_u
              i32.store8
              local.get 5
              i32.const 2
              i32.add
              local.get 14
              i32.const 2
              i32.add
              i32.load8_u
              i32.store8
              local.get 2
              local.get 4
              i32.sub
              i32.const -3
              i32.add
              local.set 8
              local.get 0
              local.get 9
              i32.const -17
              local.get 9
              i32.const -17
              i32.gt_u
              select
              local.get 15
              i32.add
              i32.const -16
              i32.and
              i32.add
              local.get 4
              i32.add
              i32.const 3
              i32.add
              local.set 11
              i32.const 0
              local.set 3
              block  ;; label = @6
                loop  ;; label = @7
                  local.get 8
                  i32.const 17
                  i32.lt_u
                  br_if 1 (;@6;)
                  local.get 5
                  local.get 3
                  i32.add
                  local.tee 6
                  i32.const 3
                  i32.add
                  local.get 14
                  local.get 3
                  i32.add
                  local.tee 12
                  i32.const 4
                  i32.add
                  i32.load
                  local.tee 7
                  i32.const 8
                  i32.shl
                  local.get 10
                  i32.const 24
                  i32.shr_u
                  i32.or
                  i32.store
                  local.get 6
                  i32.const 7
                  i32.add
                  local.get 12
                  i32.const 8
                  i32.add
                  i32.load
                  local.tee 10
                  i32.const 8
                  i32.shl
                  local.get 7
                  i32.const 24
                  i32.shr_u
                  i32.or
                  i32.store
                  local.get 6
                  i32.const 11
                  i32.add
                  local.get 12
                  i32.const 12
                  i32.add
                  i32.load
                  local.tee 7
                  i32.const 8
                  i32.shl
                  local.get 10
                  i32.const 24
                  i32.shr_u
                  i32.or
                  i32.store
                  local.get 6
                  i32.const 15
                  i32.add
                  local.get 12
                  i32.const 16
                  i32.add
                  i32.load
                  local.tee 10
                  i32.const 8
                  i32.shl
                  local.get 7
                  i32.const 24
                  i32.shr_u
                  i32.or
                  i32.store
                  local.get 3
                  i32.const 16
                  i32.add
                  local.set 3
                  local.get 8
                  i32.const -16
                  i32.add
                  local.set 8
                  br 0 (;@7;)
                end
                unreachable
              end
              local.get 1
              local.get 9
              i32.const -17
              local.get 9
              i32.const -17
              i32.gt_u
              select
              local.get 15
              i32.add
              i32.const -16
              i32.and
              local.tee 5
              i32.add
              local.get 4
              i32.add
              i32.const 3
              i32.add
              local.set 3
              i32.const -3
              local.get 5
              i32.sub
              local.get 2
              i32.add
              local.get 4
              i32.sub
              local.set 13
              br 1 (;@4;)
            end
            local.get 5
            local.get 1
            local.get 4
            i32.add
            local.tee 15
            i32.load
            local.tee 9
            i32.store8
            local.get 5
            i32.const 1
            i32.add
            local.get 15
            i32.const 1
            i32.add
            i32.load8_u
            i32.store8
            local.get 2
            local.get 4
            i32.sub
            i32.const -2
            i32.add
            local.set 8
            local.get 0
            local.get 10
            i32.const -18
            local.get 10
            i32.const -18
            i32.gt_u
            select
            local.get 14
            i32.add
            i32.const -16
            i32.and
            i32.add
            local.get 4
            i32.add
            i32.const 2
            i32.add
            local.set 11
            i32.const 0
            local.set 3
            block  ;; label = @5
              loop  ;; label = @6
                local.get 8
                i32.const 18
                i32.lt_u
                br_if 1 (;@5;)
                local.get 5
                local.get 3
                i32.add
                local.tee 6
                i32.const 2
                i32.add
                local.get 15
                local.get 3
                i32.add
                local.tee 12
                i32.const 4
                i32.add
                i32.load
                local.tee 7
                i32.const 16
                i32.shl
                local.get 9
                i32.const 16
                i32.shr_u
                i32.or
                i32.store
                local.get 6
                i32.const 6
                i32.add
                local.get 12
                i32.const 8
                i32.add
                i32.load
                local.tee 9
                i32.const 16
                i32.shl
                local.get 7
                i32.const 16
                i32.shr_u
                i32.or
                i32.store
                local.get 6
                i32.const 10
                i32.add
                local.get 12
                i32.const 12
                i32.add
                i32.load
                local.tee 7
                i32.const 16
                i32.shl
                local.get 9
                i32.const 16
                i32.shr_u
                i32.or
                i32.store
                local.get 6
                i32.const 14
                i32.add
                local.get 12
                i32.const 16
                i32.add
                i32.load
                local.tee 9
                i32.const 16
                i32.shl
                local.get 7
                i32.const 16
                i32.shr_u
                i32.or
                i32.store
                local.get 3
                i32.const 16
                i32.add
                local.set 3
                local.get 8
                i32.const -16
                i32.add
                local.set 8
                br 0 (;@6;)
              end
              unreachable
            end
            local.get 1
            local.get 10
            i32.const -18
            local.get 10
            i32.const -18
            i32.gt_u
            select
            local.get 14
            i32.add
            i32.const -16
            i32.and
            local.tee 5
            i32.add
            local.get 4
            i32.add
            i32.const 2
            i32.add
            local.set 3
            i32.const -2
            local.get 5
            i32.sub
            local.get 2
            i32.add
            local.get 4
            i32.sub
            local.set 13
          end
          local.get 13
          i32.const 16
          i32.and
          if  ;; label = @4
            local.get 11
            local.get 3
            i64.load align=1
            i64.store align=1
            local.get 11
            local.get 3
            i64.load offset=8 align=1
            i64.store offset=8 align=1
            local.get 11
            i32.const 16
            i32.add
            local.set 11
            local.get 3
            i32.const 16
            i32.add
            local.set 3
          end
          local.get 13
          i32.const 8
          i32.and
          if  ;; label = @4
            local.get 11
            local.get 3
            i64.load align=1
            i64.store align=1
            local.get 11
            i32.const 8
            i32.add
            local.set 11
            local.get 3
            i32.const 8
            i32.add
            local.set 3
          end
          local.get 13
          i32.const 4
          i32.and
          if  ;; label = @4
            local.get 11
            local.get 3
            i32.load align=1
            i32.store align=1
            local.get 11
            i32.const 4
            i32.add
            local.set 11
            local.get 3
            i32.const 4
            i32.add
            local.set 3
          end
          local.get 13
          i32.const 2
          i32.and
          if  ;; label = @4
            local.get 11
            local.get 3
            i32.load16_u align=1
            i32.store16 align=1
            local.get 11
            i32.const 2
            i32.add
            local.set 11
            local.get 3
            i32.const 2
            i32.add
            local.set 3
          end
          local.get 13
          i32.const 1
          i32.and
          i32.eqz
          br_if 0 (;@3;)
          local.get 11
          local.get 3
          i32.load8_u
          i32.store8
          local.get 0
          return
        end
        local.get 0
      end
    end)
  (func $.memset (type 0) (param i32 i32 i32) (result i32)
    (local i32 i32 i64)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        block  ;; label = @3
          local.get 2
          i32.eqz
          br_if 0 (;@3;)
          local.get 0
          local.get 2
          i32.add
          local.tee 3
          i32.const -1
          i32.add
          local.get 1
          i32.store8
          local.get 0
          local.get 1
          i32.store8
          local.get 2
          i32.const 3
          i32.lt_u
          br_if 0 (;@3;)
          local.get 3
          i32.const -2
          i32.add
          local.get 1
          i32.store8
          local.get 0
          local.get 1
          i32.store8 offset=1
          local.get 3
          i32.const -3
          i32.add
          local.get 1
          i32.store8
          local.get 0
          local.get 1
          i32.store8 offset=2
          local.get 2
          i32.const 7
          i32.lt_u
          br_if 0 (;@3;)
          local.get 3
          i32.const -4
          i32.add
          local.get 1
          i32.store8
          local.get 0
          local.get 1
          i32.store8 offset=3
          local.get 2
          i32.const 9
          i32.lt_u
          br_if 0 (;@3;)
          local.get 0
          i32.const 0
          local.get 0
          i32.sub
          i32.const 3
          i32.and
          local.tee 4
          i32.add
          local.tee 3
          local.get 1
          i32.const 255
          i32.and
          i32.const 16843009
          i32.mul
          local.tee 1
          i32.store
          local.get 3
          local.get 2
          local.get 4
          i32.sub
          i32.const -4
          i32.and
          local.tee 4
          i32.add
          local.tee 2
          i32.const -4
          i32.add
          local.get 1
          i32.store
          local.get 4
          i32.const 9
          i32.lt_u
          br_if 0 (;@3;)
          local.get 3
          local.get 1
          i32.store offset=8
          local.get 3
          local.get 1
          i32.store offset=4
          local.get 2
          i32.const -8
          i32.add
          local.get 1
          i32.store
          local.get 2
          i32.const -12
          i32.add
          local.get 1
          i32.store
          local.get 4
          i32.const 25
          i32.lt_u
          br_if 0 (;@3;)
          local.get 3
          local.get 1
          i32.store offset=16
          local.get 3
          local.get 1
          i32.store offset=12
          local.get 3
          local.get 1
          i32.store offset=20
          local.get 3
          local.get 1
          i32.store offset=24
          local.get 2
          i32.const -24
          i32.add
          local.get 1
          i32.store
          local.get 2
          i32.const -28
          i32.add
          local.get 1
          i32.store
          local.get 2
          i32.const -20
          i32.add
          local.get 1
          i32.store
          local.get 2
          i32.const -16
          i32.add
          local.get 1
          i32.store
          local.get 1
          i64.extend_i32_u
          local.tee 5
          i64.const 32
          i64.shl
          local.get 5
          i64.or
          local.set 5
          local.get 4
          local.get 3
          i32.const 4
          i32.and
          i32.const 24
          i32.or
          local.tee 1
          i32.sub
          local.set 2
          local.get 3
          local.get 1
          i32.add
          local.set 1
          loop  ;; label = @4
            local.get 2
            i32.const 32
            i32.lt_u
            br_if 1 (;@3;)
            local.get 1
            local.get 5
            i64.store
            local.get 1
            i32.const 8
            i32.add
            local.get 5
            i64.store
            local.get 1
            i32.const 16
            i32.add
            local.get 5
            i64.store
            local.get 1
            i32.const 24
            i32.add
            local.get 5
            i64.store
            local.get 1
            i32.const 32
            i32.add
            local.set 1
            local.get 2
            i32.const -32
            i32.add
            local.set 2
            br 0 (;@4;)
          end
          unreachable
        end
        local.get 0
      end
    end)
  (func $.init_mparams (type 2)
    (local i32)
    block  ;; label = @1
      block  ;; label = @2
        i32.const 12
        i32.load
        if  ;; label = @3
          return
        end
        i32.const 4
        i32.load
        i32.const 16
        i32.sub
        local.set 0
        i32.const 16
        i64.const 281474976776192
        i64.store align=4
        i32.const 24
        i64.const -1
        i64.store align=4
        i32.const 12
        local.get 0
        i32.const 12
        i32.add
        i32.const -16
        i32.and
        i32.const 1431655768
        i32.xor
        i32.store
        i32.const 32
        i32.const 0
        i32.store
      end
    end)
  (func $.mspace_malloc (type 1) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                block  ;; label = @7
                  block  ;; label = @8
                    block  ;; label = @9
                      block  ;; label = @10
                        block  ;; label = @11
                          block  ;; label = @12
                            block  ;; label = @13
                              block  ;; label = @14
                                block  ;; label = @15
                                  block  ;; label = @16
                                    block  ;; label = @17
                                      block  ;; label = @18
                                        block  ;; label = @19
                                          block  ;; label = @20
                                            block  ;; label = @21
                                              block  ;; label = @22
                                                block  ;; label = @23
                                                  block  ;; label = @24
                                                    block  ;; label = @25
                                                      block  ;; label = @26
                                                        block (result i32)  ;; label = @27
                                                          block  ;; label = @28
                                                            block  ;; label = @29
                                                              block  ;; label = @30
                                                                block  ;; label = @31
                                                                  block  ;; label = @32
                                                                    block  ;; label = @33
                                                                      block  ;; label = @34
                                                                        block  ;; label = @35
                                                                          block  ;; label = @36
                                                                            block  ;; label = @37
                                                                              block  ;; label = @38
                                                                                block  ;; label = @39
                                                                                  block  ;; label = @40
                                                                                    block  ;; label = @41
                                                                                      block  ;; label = @42
                                                                                        block  ;; label = @43
                                                                                          local.get 1
                                                                                          i32.const 244
                                                                                          i32.le_u
                                                                                          if  ;; label = @44
                                                                                            local.get 0
                                                                                            i32.load
                                                                                            local.tee 4
                                                                                            i32.const 16
                                                                                            local.get 1
                                                                                            i32.const 11
                                                                                            i32.add
                                                                                            i32.const -8
                                                                                            i32.and
                                                                                            local.get 1
                                                                                            i32.const 11
                                                                                            i32.lt_u
                                                                                            select
                                                                                            local.tee 5
                                                                                            i32.const 3
                                                                                            i32.shr_u
                                                                                            local.tee 2
                                                                                            i32.shr_u
                                                                                            local.tee 1
                                                                                            i32.const 3
                                                                                            i32.and
                                                                                            i32.eqz
                                                                                            br_if 1 (;@43;)
                                                                                            local.get 0
                                                                                            local.get 1
                                                                                            i32.const -1
                                                                                            i32.xor
                                                                                            i32.const 1
                                                                                            i32.and
                                                                                            local.get 2
                                                                                            i32.add
                                                                                            local.tee 2
                                                                                            i32.const 3
                                                                                            i32.shl
                                                                                            i32.add
                                                                                            local.tee 3
                                                                                            i32.const 48
                                                                                            i32.add
                                                                                            i32.load
                                                                                            local.tee 1
                                                                                            i32.const 8
                                                                                            i32.add
                                                                                            local.set 6
                                                                                            local.get 1
                                                                                            i32.load offset=8
                                                                                            local.tee 5
                                                                                            local.get 3
                                                                                            i32.const 40
                                                                                            i32.add
                                                                                            local.tee 3
                                                                                            i32.eq
                                                                                            br_if 2 (;@42;)
                                                                                            local.get 5
                                                                                            local.get 3
                                                                                            i32.store offset=12
                                                                                            local.get 3
                                                                                            i32.const 8
                                                                                            i32.add
                                                                                            local.get 5
                                                                                            i32.store
                                                                                            br 3 (;@41;)
                                                                                          end
                                                                                          i32.const -1
                                                                                          local.set 5
                                                                                          local.get 1
                                                                                          i32.const -65
                                                                                          i32.gt_u
                                                                                          br_if 9 (;@34;)
                                                                                          local.get 1
                                                                                          i32.const 11
                                                                                          i32.add
                                                                                          local.tee 1
                                                                                          i32.const -8
                                                                                          i32.and
                                                                                          local.set 5
                                                                                          local.get 0
                                                                                          i32.load offset=4
                                                                                          local.tee 9
                                                                                          i32.eqz
                                                                                          br_if 9 (;@34;)
                                                                                          i32.const 0
                                                                                          local.set 4
                                                                                          block (result i32)  ;; label = @44
                                                                                            i32.const 0
                                                                                            local.get 1
                                                                                            i32.const 8
                                                                                            i32.shr_u
                                                                                            local.tee 1
                                                                                            i32.eqz
                                                                                            br_if 0 (;@44;)
                                                                                            drop
                                                                                            i32.const 31
                                                                                            local.get 5
                                                                                            i32.const 16777215
                                                                                            i32.gt_u
                                                                                            br_if 0 (;@44;)
                                                                                            drop
                                                                                            local.get 5
                                                                                            i32.const 14
                                                                                            local.get 1
                                                                                            local.get 1
                                                                                            i32.const 1048320
                                                                                            i32.add
                                                                                            i32.const 16
                                                                                            i32.shr_u
                                                                                            i32.const 8
                                                                                            i32.and
                                                                                            local.tee 2
                                                                                            i32.shl
                                                                                            local.tee 1
                                                                                            i32.const 520192
                                                                                            i32.add
                                                                                            i32.const 16
                                                                                            i32.shr_u
                                                                                            i32.const 4
                                                                                            i32.and
                                                                                            local.tee 3
                                                                                            local.get 2
                                                                                            i32.or
                                                                                            local.get 1
                                                                                            local.get 3
                                                                                            i32.shl
                                                                                            local.tee 1
                                                                                            i32.const 245760
                                                                                            i32.add
                                                                                            i32.const 16
                                                                                            i32.shr_u
                                                                                            i32.const 2
                                                                                            i32.and
                                                                                            local.tee 2
                                                                                            i32.or
                                                                                            i32.sub
                                                                                            local.get 1
                                                                                            local.get 2
                                                                                            i32.shl
                                                                                            i32.const 15
                                                                                            i32.shr_u
                                                                                            i32.add
                                                                                            local.tee 1
                                                                                            i32.const 7
                                                                                            i32.add
                                                                                            i32.shr_u
                                                                                            i32.const 1
                                                                                            i32.and
                                                                                            local.get 1
                                                                                            i32.const 1
                                                                                            i32.shl
                                                                                            i32.or
                                                                                          end
                                                                                          local.set 7
                                                                                          i32.const 0
                                                                                          local.get 5
                                                                                          i32.sub
                                                                                          local.set 2
                                                                                          local.get 0
                                                                                          local.get 7
                                                                                          i32.const 2
                                                                                          i32.shl
                                                                                          i32.add
                                                                                          i32.const 304
                                                                                          i32.add
                                                                                          i32.load
                                                                                          local.tee 1
                                                                                          i32.eqz
                                                                                          br_if 3 (;@40;)
                                                                                          local.get 5
                                                                                          i32.const 0
                                                                                          i32.const 25
                                                                                          local.get 7
                                                                                          i32.const 1
                                                                                          i32.shr_u
                                                                                          i32.sub
                                                                                          local.get 7
                                                                                          i32.const 31
                                                                                          i32.eq
                                                                                          select
                                                                                          i32.shl
                                                                                          local.set 6
                                                                                          i32.const 0
                                                                                          local.set 4
                                                                                          i32.const 0
                                                                                          local.set 3
                                                                                          loop  ;; label = @44
                                                                                            local.get 1
                                                                                            i32.load offset=4
                                                                                            i32.const -8
                                                                                            i32.and
                                                                                            local.get 5
                                                                                            i32.sub
                                                                                            local.tee 8
                                                                                            local.get 2
                                                                                            i32.lt_u
                                                                                            if  ;; label = @45
                                                                                              local.get 8
                                                                                              local.set 2
                                                                                              local.get 1
                                                                                              local.set 3
                                                                                              local.get 8
                                                                                              i32.eqz
                                                                                              br_if 8 (;@37;)
                                                                                            end
                                                                                            local.get 4
                                                                                            local.get 1
                                                                                            i32.const 20
                                                                                            i32.add
                                                                                            i32.load
                                                                                            local.tee 8
                                                                                            local.get 8
                                                                                            local.get 1
                                                                                            local.get 6
                                                                                            i32.const 29
                                                                                            i32.shr_u
                                                                                            i32.const 4
                                                                                            i32.and
                                                                                            i32.add
                                                                                            i32.const 16
                                                                                            i32.add
                                                                                            i32.load
                                                                                            local.tee 1
                                                                                            i32.eq
                                                                                            select
                                                                                            local.get 4
                                                                                            local.get 8
                                                                                            select
                                                                                            local.set 4
                                                                                            local.get 6
                                                                                            local.get 1
                                                                                            i32.const 0
                                                                                            i32.ne
                                                                                            i32.shl
                                                                                            local.set 6
                                                                                            local.get 1
                                                                                            br_if 0 (;@44;)
                                                                                            br 5 (;@39;)
                                                                                          end
                                                                                          unreachable
                                                                                        end
                                                                                        local.get 5
                                                                                        local.get 0
                                                                                        i32.load offset=8
                                                                                        local.tee 3
                                                                                        i32.le_u
                                                                                        br_if 8 (;@34;)
                                                                                        local.get 1
                                                                                        i32.eqz
                                                                                        br_if 4 (;@38;)
                                                                                        local.get 0
                                                                                        i32.const 40
                                                                                        i32.add
                                                                                        local.tee 7
                                                                                        local.get 1
                                                                                        local.get 2
                                                                                        i32.shl
                                                                                        i32.const 2
                                                                                        local.get 2
                                                                                        i32.shl
                                                                                        local.tee 1
                                                                                        i32.const 0
                                                                                        local.get 1
                                                                                        i32.sub
                                                                                        i32.or
                                                                                        i32.and
                                                                                        local.tee 1
                                                                                        i32.const 0
                                                                                        local.get 1
                                                                                        i32.sub
                                                                                        i32.and
                                                                                        i32.const -1
                                                                                        i32.add
                                                                                        local.tee 1
                                                                                        local.get 1
                                                                                        i32.const 12
                                                                                        i32.shr_u
                                                                                        i32.const 16
                                                                                        i32.and
                                                                                        local.tee 1
                                                                                        i32.shr_u
                                                                                        local.tee 2
                                                                                        i32.const 5
                                                                                        i32.shr_u
                                                                                        i32.const 8
                                                                                        i32.and
                                                                                        local.tee 6
                                                                                        local.get 1
                                                                                        i32.or
                                                                                        local.get 2
                                                                                        local.get 6
                                                                                        i32.shr_u
                                                                                        local.tee 1
                                                                                        i32.const 2
                                                                                        i32.shr_u
                                                                                        i32.const 4
                                                                                        i32.and
                                                                                        local.tee 2
                                                                                        i32.or
                                                                                        local.get 1
                                                                                        local.get 2
                                                                                        i32.shr_u
                                                                                        local.tee 1
                                                                                        i32.const 1
                                                                                        i32.shr_u
                                                                                        i32.const 2
                                                                                        i32.and
                                                                                        local.tee 2
                                                                                        i32.or
                                                                                        local.get 1
                                                                                        local.get 2
                                                                                        i32.shr_u
                                                                                        local.tee 1
                                                                                        i32.const 1
                                                                                        i32.shr_u
                                                                                        i32.const 1
                                                                                        i32.and
                                                                                        local.tee 2
                                                                                        i32.or
                                                                                        local.get 1
                                                                                        local.get 2
                                                                                        i32.shr_u
                                                                                        i32.add
                                                                                        local.tee 6
                                                                                        i32.const 3
                                                                                        i32.shl
                                                                                        i32.add
                                                                                        local.tee 2
                                                                                        i32.load offset=8
                                                                                        local.tee 1
                                                                                        i32.load offset=8
                                                                                        local.tee 8
                                                                                        local.get 2
                                                                                        i32.eq
                                                                                        br_if 9 (;@33;)
                                                                                        local.get 2
                                                                                        i32.const 8
                                                                                        i32.add
                                                                                        local.get 8
                                                                                        i32.store
                                                                                        local.get 8
                                                                                        local.get 2
                                                                                        i32.store offset=12
                                                                                        local.get 0
                                                                                        i32.const 8
                                                                                        i32.add
                                                                                        i32.load
                                                                                        local.set 3
                                                                                        br 10 (;@32;)
                                                                                      end
                                                                                      local.get 0
                                                                                      local.get 4
                                                                                      i32.const -2
                                                                                      local.get 2
                                                                                      i32.rotl
                                                                                      i32.and
                                                                                      i32.store
                                                                                    end
                                                                                    local.get 1
                                                                                    local.get 2
                                                                                    i32.const 3
                                                                                    i32.shl
                                                                                    local.tee 2
                                                                                    i32.const 3
                                                                                    i32.or
                                                                                    i32.store offset=4
                                                                                    local.get 1
                                                                                    local.get 2
                                                                                    i32.add
                                                                                    local.tee 1
                                                                                    local.get 1
                                                                                    i32.load offset=4
                                                                                    i32.const 1
                                                                                    i32.or
                                                                                    i32.store offset=4
                                                                                    local.get 6
                                                                                    return
                                                                                  end
                                                                                  i32.const 0
                                                                                  local.set 3
                                                                                end
                                                                                local.get 4
                                                                                local.get 3
                                                                                i32.or
                                                                                i32.eqz
                                                                                if  ;; label = @39
                                                                                  i32.const 0
                                                                                  local.set 1
                                                                                  local.get 9
                                                                                  i32.const 2
                                                                                  local.get 7
                                                                                  i32.shl
                                                                                  local.tee 3
                                                                                  i32.const 0
                                                                                  local.get 3
                                                                                  i32.sub
                                                                                  i32.or
                                                                                  i32.and
                                                                                  local.tee 4
                                                                                  i32.eqz
                                                                                  br_if 3 (;@36;)
                                                                                  i32.const 0
                                                                                  local.set 3
                                                                                  local.get 0
                                                                                  local.get 4
                                                                                  i32.const 0
                                                                                  local.get 4
                                                                                  i32.sub
                                                                                  i32.and
                                                                                  i32.const -1
                                                                                  i32.add
                                                                                  local.tee 1
                                                                                  local.get 1
                                                                                  i32.const 12
                                                                                  i32.shr_u
                                                                                  i32.const 16
                                                                                  i32.and
                                                                                  local.tee 1
                                                                                  i32.shr_u
                                                                                  local.tee 4
                                                                                  i32.const 5
                                                                                  i32.shr_u
                                                                                  i32.const 8
                                                                                  i32.and
                                                                                  local.tee 6
                                                                                  local.get 1
                                                                                  i32.or
                                                                                  local.get 4
                                                                                  local.get 6
                                                                                  i32.shr_u
                                                                                  local.tee 1
                                                                                  i32.const 2
                                                                                  i32.shr_u
                                                                                  i32.const 4
                                                                                  i32.and
                                                                                  local.tee 4
                                                                                  i32.or
                                                                                  local.get 1
                                                                                  local.get 4
                                                                                  i32.shr_u
                                                                                  local.tee 1
                                                                                  i32.const 1
                                                                                  i32.shr_u
                                                                                  i32.const 2
                                                                                  i32.and
                                                                                  local.tee 4
                                                                                  i32.or
                                                                                  local.get 1
                                                                                  local.get 4
                                                                                  i32.shr_u
                                                                                  local.tee 1
                                                                                  i32.const 1
                                                                                  i32.shr_u
                                                                                  i32.const 1
                                                                                  i32.and
                                                                                  local.tee 4
                                                                                  i32.or
                                                                                  local.get 1
                                                                                  local.get 4
                                                                                  i32.shr_u
                                                                                  i32.add
                                                                                  i32.const 2
                                                                                  i32.shl
                                                                                  i32.add
                                                                                  i32.const 304
                                                                                  i32.add
                                                                                  i32.load
                                                                                  local.set 1
                                                                                  br 4 (;@35;)
                                                                                end
                                                                                local.get 4
                                                                                local.set 1
                                                                                br 3 (;@35;)
                                                                              end
                                                                              local.get 0
                                                                              i32.load offset=4
                                                                              local.tee 1
                                                                              i32.eqz
                                                                              br_if 3 (;@34;)
                                                                              local.get 0
                                                                              local.get 1
                                                                              i32.const 0
                                                                              local.get 1
                                                                              i32.sub
                                                                              i32.and
                                                                              i32.const -1
                                                                              i32.add
                                                                              local.tee 1
                                                                              local.get 1
                                                                              i32.const 12
                                                                              i32.shr_u
                                                                              i32.const 16
                                                                              i32.and
                                                                              local.tee 1
                                                                              i32.shr_u
                                                                              local.tee 2
                                                                              i32.const 5
                                                                              i32.shr_u
                                                                              i32.const 8
                                                                              i32.and
                                                                              local.tee 3
                                                                              local.get 1
                                                                              i32.or
                                                                              local.get 2
                                                                              local.get 3
                                                                              i32.shr_u
                                                                              local.tee 1
                                                                              i32.const 2
                                                                              i32.shr_u
                                                                              i32.const 4
                                                                              i32.and
                                                                              local.tee 2
                                                                              i32.or
                                                                              local.get 1
                                                                              local.get 2
                                                                              i32.shr_u
                                                                              local.tee 1
                                                                              i32.const 1
                                                                              i32.shr_u
                                                                              i32.const 2
                                                                              i32.and
                                                                              local.tee 2
                                                                              i32.or
                                                                              local.get 1
                                                                              local.get 2
                                                                              i32.shr_u
                                                                              local.tee 1
                                                                              i32.const 1
                                                                              i32.shr_u
                                                                              i32.const 1
                                                                              i32.and
                                                                              local.tee 2
                                                                              i32.or
                                                                              local.get 1
                                                                              local.get 2
                                                                              i32.shr_u
                                                                              i32.add
                                                                              i32.const 2
                                                                              i32.shl
                                                                              i32.add
                                                                              i32.const 304
                                                                              i32.add
                                                                              i32.load
                                                                              local.tee 3
                                                                              i32.load offset=4
                                                                              i32.const -8
                                                                              i32.and
                                                                              local.get 5
                                                                              i32.sub
                                                                              local.set 2
                                                                              local.get 3
                                                                              local.set 1
                                                                              block  ;; label = @38
                                                                                loop  ;; label = @39
                                                                                  local.get 1
                                                                                  i32.const 16
                                                                                  i32.add
                                                                                  local.get 1
                                                                                  i32.load offset=16
                                                                                  i32.eqz
                                                                                  i32.const 2
                                                                                  i32.shl
                                                                                  i32.add
                                                                                  i32.load
                                                                                  local.tee 1
                                                                                  i32.eqz
                                                                                  br_if 1 (;@38;)
                                                                                  local.get 1
                                                                                  i32.load offset=4
                                                                                  i32.const -8
                                                                                  i32.and
                                                                                  local.get 5
                                                                                  i32.sub
                                                                                  local.tee 4
                                                                                  local.get 2
                                                                                  local.get 4
                                                                                  local.get 2
                                                                                  i32.lt_u
                                                                                  local.tee 4
                                                                                  select
                                                                                  local.set 2
                                                                                  local.get 1
                                                                                  local.get 3
                                                                                  local.get 4
                                                                                  select
                                                                                  local.set 3
                                                                                  br 0 (;@39;)
                                                                                end
                                                                                unreachable
                                                                              end
                                                                              local.get 3
                                                                              local.get 5
                                                                              i32.add
                                                                              local.tee 9
                                                                              local.get 3
                                                                              i32.le_u
                                                                              br_if 3 (;@34;)
                                                                              local.get 3
                                                                              i32.load offset=24
                                                                              local.set 7
                                                                              local.get 3
                                                                              i32.load offset=12
                                                                              local.tee 6
                                                                              local.get 3
                                                                              i32.eq
                                                                              br_if 12 (;@25;)
                                                                              local.get 3
                                                                              i32.load offset=8
                                                                              local.tee 1
                                                                              local.get 6
                                                                              i32.store offset=12
                                                                              local.get 6
                                                                              local.get 1
                                                                              i32.store offset=8
                                                                              local.get 7
                                                                              br_if 31 (;@6;)
                                                                              br 32 (;@5;)
                                                                            end
                                                                            i32.const 0
                                                                            local.set 2
                                                                            local.get 1
                                                                            local.set 3
                                                                            br 1 (;@35;)
                                                                          end
                                                                          i32.const 0
                                                                          local.set 3
                                                                        end
                                                                        block  ;; label = @35
                                                                          loop  ;; label = @36
                                                                            local.get 1
                                                                            i32.eqz
                                                                            br_if 1 (;@35;)
                                                                            local.get 1
                                                                            i32.load offset=4
                                                                            i32.const -8
                                                                            i32.and
                                                                            local.get 5
                                                                            i32.sub
                                                                            local.tee 4
                                                                            local.get 2
                                                                            local.get 4
                                                                            local.get 2
                                                                            i32.lt_u
                                                                            local.tee 4
                                                                            select
                                                                            local.set 2
                                                                            local.get 1
                                                                            local.get 3
                                                                            local.get 4
                                                                            select
                                                                            local.set 3
                                                                            local.get 1
                                                                            i32.const 16
                                                                            i32.add
                                                                            local.get 1
                                                                            i32.load offset=16
                                                                            i32.eqz
                                                                            i32.const 2
                                                                            i32.shl
                                                                            i32.add
                                                                            i32.load
                                                                            local.set 1
                                                                            br 0 (;@36;)
                                                                          end
                                                                          unreachable
                                                                        end
                                                                        local.get 3
                                                                        i32.eqz
                                                                        local.get 2
                                                                        local.get 0
                                                                        i32.load offset=8
                                                                        local.get 5
                                                                        i32.sub
                                                                        i32.ge_u
                                                                        i32.or
                                                                        br_if 0 (;@34;)
                                                                        local.get 3
                                                                        local.get 5
                                                                        i32.add
                                                                        local.tee 7
                                                                        local.get 3
                                                                        i32.le_u
                                                                        br_if 22 (;@12;)
                                                                        local.get 3
                                                                        i32.load offset=24
                                                                        local.set 9
                                                                        local.get 3
                                                                        i32.load offset=12
                                                                        local.tee 6
                                                                        local.get 3
                                                                        i32.eq
                                                                        br_if 3 (;@31;)
                                                                        local.get 3
                                                                        i32.load offset=8
                                                                        local.tee 1
                                                                        local.get 6
                                                                        i32.store offset=12
                                                                        local.get 6
                                                                        local.get 1
                                                                        i32.store offset=8
                                                                        local.get 9
                                                                        br_if 30 (;@4;)
                                                                        br 31 (;@3;)
                                                                      end
                                                                      block  ;; label = @34
                                                                        block  ;; label = @35
                                                                          block  ;; label = @36
                                                                            local.get 0
                                                                            i32.load offset=8
                                                                            local.tee 1
                                                                            local.get 5
                                                                            i32.lt_u
                                                                            if  ;; label = @37
                                                                              local.get 0
                                                                              i32.load offset=12
                                                                              local.tee 1
                                                                              local.get 5
                                                                              i32.le_u
                                                                              br_if 1 (;@36;)
                                                                              local.get 0
                                                                              i32.load offset=24
                                                                              local.tee 2
                                                                              local.get 5
                                                                              i32.add
                                                                              local.tee 3
                                                                              local.get 1
                                                                              local.get 5
                                                                              i32.sub
                                                                              local.tee 1
                                                                              i32.const 1
                                                                              i32.or
                                                                              i32.store offset=4
                                                                              local.get 0
                                                                              i32.const 12
                                                                              i32.add
                                                                              local.get 1
                                                                              i32.store
                                                                              local.get 0
                                                                              local.get 3
                                                                              i32.store offset=24
                                                                              local.get 2
                                                                              local.get 5
                                                                              i32.const 3
                                                                              i32.or
                                                                              i32.store offset=4
                                                                              local.get 2
                                                                              i32.const 8
                                                                              i32.add
                                                                              return
                                                                            end
                                                                            local.get 0
                                                                            i32.load offset=20
                                                                            local.set 2
                                                                            local.get 1
                                                                            local.get 5
                                                                            i32.sub
                                                                            local.tee 3
                                                                            i32.const 16
                                                                            i32.lt_u
                                                                            br_if 1 (;@35;)
                                                                            local.get 2
                                                                            local.get 5
                                                                            i32.add
                                                                            local.tee 4
                                                                            local.get 3
                                                                            i32.const 1
                                                                            i32.or
                                                                            i32.store offset=4
                                                                            local.get 2
                                                                            local.get 1
                                                                            i32.add
                                                                            local.get 3
                                                                            i32.store
                                                                            local.get 0
                                                                            i32.const 8
                                                                            i32.add
                                                                            local.get 3
                                                                            i32.store
                                                                            local.get 0
                                                                            i32.const 20
                                                                            i32.add
                                                                            local.get 4
                                                                            i32.store
                                                                            local.get 2
                                                                            local.get 5
                                                                            i32.const 3
                                                                            i32.or
                                                                            i32.store offset=4
                                                                            br 2 (;@34;)
                                                                          end
                                                                          i32.const 0
                                                                          local.set 6
                                                                          i32.const 12
                                                                          i32.load
                                                                          i32.eqz
                                                                          if  ;; label = @36
                                                                            call $.init_mparams
                                                                          end
                                                                          i32.const 20
                                                                          i32.load
                                                                          local.tee 1
                                                                          local.get 5
                                                                          i32.const 47
                                                                          i32.add
                                                                          local.tee 4
                                                                          i32.add
                                                                          local.tee 2
                                                                          i32.const 0
                                                                          local.get 1
                                                                          i32.sub
                                                                          local.tee 3
                                                                          i32.and
                                                                          local.tee 1
                                                                          local.get 5
                                                                          i32.le_u
                                                                          br_if 18 (;@17;)
                                                                          local.get 0
                                                                          i32.load offset=440
                                                                          local.tee 7
                                                                          if  ;; label = @36
                                                                            local.get 0
                                                                            i32.load offset=432
                                                                            local.tee 8
                                                                            local.get 1
                                                                            i32.add
                                                                            local.tee 9
                                                                            local.get 8
                                                                            i32.le_u
                                                                            local.get 9
                                                                            local.get 7
                                                                            i32.gt_u
                                                                            i32.or
                                                                            br_if 19 (;@17;)
                                                                          end
                                                                          local.get 0
                                                                          i32.const 444
                                                                          i32.add
                                                                          i32.load8_u
                                                                          i32.const 4
                                                                          i32.and
                                                                          br_if 16 (;@19;)
                                                                          local.get 0
                                                                          i32.load offset=24
                                                                          local.tee 8
                                                                          i32.eqz
                                                                          br_if 5 (;@30;)
                                                                          local.get 0
                                                                          local.get 8
                                                                          call $.segment_holding
                                                                          local.tee 9
                                                                          i32.eqz
                                                                          br_if 5 (;@30;)
                                                                          local.get 2
                                                                          local.get 0
                                                                          i32.const 12
                                                                          i32.add
                                                                          i32.load
                                                                          i32.sub
                                                                          local.get 3
                                                                          i32.and
                                                                          local.tee 8
                                                                          i32.const 2147483646
                                                                          i32.gt_u
                                                                          br_if 15 (;@20;)
                                                                          local.get 8
                                                                          call $.morecore
                                                                          local.tee 3
                                                                          local.get 9
                                                                          i32.load
                                                                          local.get 9
                                                                          i32.load offset=4
                                                                          i32.add
                                                                          i32.ne
                                                                          br_if 6 (;@29;)
                                                                          local.get 3
                                                                          i32.const -1
                                                                          i32.ne
                                                                          br_if 17 (;@18;)
                                                                          br 15 (;@20;)
                                                                        end
                                                                        local.get 2
                                                                        local.get 1
                                                                        i32.const 3
                                                                        i32.or
                                                                        i32.store offset=4
                                                                        local.get 0
                                                                        i32.const 20
                                                                        i32.add
                                                                        i32.const 0
                                                                        i32.store
                                                                        local.get 0
                                                                        i32.const 8
                                                                        i32.add
                                                                        i32.const 0
                                                                        i32.store
                                                                        local.get 2
                                                                        local.get 1
                                                                        i32.add
                                                                        local.tee 1
                                                                        local.get 1
                                                                        i32.load offset=4
                                                                        i32.const 1
                                                                        i32.or
                                                                        i32.store offset=4
                                                                      end
                                                                      local.get 2
                                                                      i32.const 8
                                                                      i32.add
                                                                      return
                                                                    end
                                                                    local.get 0
                                                                    local.get 4
                                                                    i32.const -2
                                                                    local.get 6
                                                                    i32.rotl
                                                                    i32.and
                                                                    i32.store
                                                                  end
                                                                  local.get 1
                                                                  i32.const 8
                                                                  i32.add
                                                                  local.set 4
                                                                  local.get 1
                                                                  local.get 5
                                                                  i32.const 3
                                                                  i32.or
                                                                  i32.store offset=4
                                                                  local.get 1
                                                                  local.get 5
                                                                  i32.add
                                                                  local.tee 8
                                                                  local.get 6
                                                                  i32.const 3
                                                                  i32.shl
                                                                  local.tee 6
                                                                  local.get 5
                                                                  i32.sub
                                                                  local.tee 2
                                                                  i32.const 1
                                                                  i32.or
                                                                  i32.store offset=4
                                                                  local.get 1
                                                                  local.get 6
                                                                  i32.add
                                                                  local.get 2
                                                                  i32.store
                                                                  local.get 3
                                                                  i32.eqz
                                                                  br_if 5 (;@26;)
                                                                  local.get 7
                                                                  local.get 3
                                                                  i32.const 3
                                                                  i32.shr_u
                                                                  local.tee 3
                                                                  i32.const 3
                                                                  i32.shl
                                                                  i32.add
                                                                  local.set 5
                                                                  local.get 0
                                                                  i32.const 20
                                                                  i32.add
                                                                  i32.load
                                                                  local.set 1
                                                                  local.get 0
                                                                  i32.load
                                                                  local.tee 6
                                                                  i32.const 1
                                                                  local.get 3
                                                                  i32.shl
                                                                  local.tee 3
                                                                  i32.and
                                                                  i32.eqz
                                                                  br_if 3 (;@28;)
                                                                  local.get 5
                                                                  i32.load offset=8
                                                                  br 4 (;@27;)
                                                                end
                                                                local.get 3
                                                                i32.const 20
                                                                i32.add
                                                                local.tee 4
                                                                i32.load
                                                                local.tee 1
                                                                i32.eqz
                                                                if  ;; label = @31
                                                                  local.get 3
                                                                  i32.load offset=16
                                                                  local.tee 1
                                                                  i32.eqz
                                                                  br_if 8 (;@23;)
                                                                  local.get 3
                                                                  i32.const 16
                                                                  i32.add
                                                                  local.set 4
                                                                end
                                                                loop  ;; label = @31
                                                                  local.get 4
                                                                  local.set 8
                                                                  local.get 1
                                                                  local.tee 6
                                                                  i32.const 20
                                                                  i32.add
                                                                  local.tee 4
                                                                  i32.load
                                                                  local.tee 1
                                                                  br_if 0 (;@31;)
                                                                  local.get 6
                                                                  i32.const 16
                                                                  i32.add
                                                                  local.set 4
                                                                  local.get 6
                                                                  i32.load offset=16
                                                                  local.tee 1
                                                                  br_if 0 (;@31;)
                                                                end
                                                                local.get 8
                                                                i32.const 0
                                                                i32.store
                                                                local.get 9
                                                                i32.eqz
                                                                br_if 27 (;@3;)
                                                                br 26 (;@4;)
                                                              end
                                                              memory.size
                                                              local.set 2
                                                              local.get 1
                                                              local.set 8
                                                              i32.const 16
                                                              i32.load
                                                              local.tee 9
                                                              i32.const -1
                                                              i32.add
                                                              local.tee 10
                                                              local.get 2
                                                              i32.const 16
                                                              i32.shl
                                                              local.tee 3
                                                              i32.and
                                                              if  ;; label = @30
                                                                local.get 1
                                                                local.get 3
                                                                i32.sub
                                                                local.get 10
                                                                local.get 3
                                                                i32.add
                                                                i32.const 0
                                                                local.get 9
                                                                i32.sub
                                                                i32.and
                                                                i32.add
                                                                local.set 8
                                                              end
                                                              local.get 8
                                                              local.get 5
                                                              i32.le_u
                                                              local.get 8
                                                              i32.const 2147483646
                                                              i32.gt_u
                                                              i32.or
                                                              br_if 9 (;@20;)
                                                              local.get 7
                                                              if  ;; label = @30
                                                                local.get 0
                                                                i32.load offset=432
                                                                local.tee 2
                                                                local.get 8
                                                                i32.add
                                                                local.tee 9
                                                                local.get 2
                                                                i32.le_u
                                                                local.get 9
                                                                local.get 7
                                                                i32.gt_u
                                                                i32.or
                                                                br_if 10 (;@20;)
                                                              end
                                                              local.get 8
                                                              call $.morecore
                                                              local.tee 2
                                                              local.get 3
                                                              i32.eq
                                                              br_if 11 (;@18;)
                                                              local.get 2
                                                              local.set 3
                                                            end
                                                            local.get 5
                                                            i32.const 48
                                                            i32.add
                                                            local.get 8
                                                            i32.le_u
                                                            local.get 8
                                                            i32.const 2147483646
                                                            i32.gt_u
                                                            i32.or
                                                            local.get 3
                                                            i32.const -1
                                                            i32.eq
                                                            i32.or
                                                            br_if 4 (;@24;)
                                                            local.get 4
                                                            local.get 8
                                                            i32.sub
                                                            i32.const 20
                                                            i32.load
                                                            local.tee 2
                                                            i32.add
                                                            i32.const 0
                                                            local.get 2
                                                            i32.sub
                                                            i32.and
                                                            local.tee 2
                                                            i32.const 2147483646
                                                            i32.gt_u
                                                            br_if 10 (;@18;)
                                                            local.get 2
                                                            call $.morecore
                                                            i32.const -1
                                                            i32.eq
                                                            br_if 7 (;@21;)
                                                            local.get 2
                                                            local.get 8
                                                            i32.add
                                                            local.set 8
                                                            br 10 (;@18;)
                                                          end
                                                          local.get 0
                                                          local.get 6
                                                          local.get 3
                                                          i32.or
                                                          i32.store
                                                          local.get 5
                                                        end
                                                        local.tee 3
                                                        local.get 1
                                                        i32.store offset=12
                                                        local.get 5
                                                        i32.const 8
                                                        i32.add
                                                        local.get 1
                                                        i32.store
                                                        local.get 1
                                                        local.get 5
                                                        i32.store offset=12
                                                        local.get 1
                                                        local.get 3
                                                        i32.store offset=8
                                                      end
                                                      local.get 0
                                                      i32.const 20
                                                      i32.add
                                                      local.get 8
                                                      i32.store
                                                      local.get 0
                                                      i32.const 8
                                                      i32.add
                                                      local.get 2
                                                      i32.store
                                                      local.get 4
                                                      return
                                                    end
                                                    local.get 3
                                                    i32.const 20
                                                    i32.add
                                                    local.tee 4
                                                    i32.load
                                                    local.tee 1
                                                    i32.eqz
                                                    if  ;; label = @25
                                                      local.get 3
                                                      i32.load offset=16
                                                      local.tee 1
                                                      i32.eqz
                                                      br_if 3 (;@22;)
                                                      local.get 3
                                                      i32.const 16
                                                      i32.add
                                                      local.set 4
                                                    end
                                                    loop  ;; label = @25
                                                      local.get 4
                                                      local.set 8
                                                      local.get 1
                                                      local.tee 6
                                                      i32.const 20
                                                      i32.add
                                                      local.tee 4
                                                      i32.load
                                                      local.tee 1
                                                      br_if 0 (;@25;)
                                                      local.get 6
                                                      i32.const 16
                                                      i32.add
                                                      local.set 4
                                                      local.get 6
                                                      i32.load offset=16
                                                      local.tee 1
                                                      br_if 0 (;@25;)
                                                    end
                                                    local.get 8
                                                    i32.const 0
                                                    i32.store
                                                    local.get 7
                                                    i32.eqz
                                                    br_if 19 (;@5;)
                                                    br 18 (;@6;)
                                                  end
                                                  local.get 3
                                                  i32.const -1
                                                  i32.ne
                                                  br_if 5 (;@18;)
                                                  br 3 (;@20;)
                                                end
                                                i32.const 0
                                                local.set 6
                                                local.get 9
                                                br_if 18 (;@4;)
                                                br 19 (;@3;)
                                              end
                                              i32.const 0
                                              local.set 6
                                              local.get 7
                                              br_if 15 (;@6;)
                                              br 16 (;@5;)
                                            end
                                            i32.const 0
                                            local.get 8
                                            i32.sub
                                            call $.morecore
                                            drop
                                          end
                                          local.get 0
                                          i32.const 444
                                          i32.add
                                          local.tee 2
                                          local.get 2
                                          i32.load
                                          i32.const 4
                                          i32.or
                                          i32.store
                                        end
                                        local.get 1
                                        i32.const 2147483646
                                        i32.gt_u
                                        br_if 1 (;@17;)
                                        local.get 1
                                        call $.morecore
                                        local.set 3
                                        memory.size
                                        local.set 1
                                        local.get 3
                                        i32.const -1
                                        i32.eq
                                        br_if 1 (;@17;)
                                        local.get 3
                                        local.get 1
                                        i32.const 16
                                        i32.shl
                                        local.tee 1
                                        i32.ge_u
                                        br_if 1 (;@17;)
                                        local.get 1
                                        local.get 3
                                        i32.sub
                                        local.tee 8
                                        local.get 5
                                        i32.const 40
                                        i32.add
                                        i32.le_u
                                        br_if 1 (;@17;)
                                      end
                                      local.get 0
                                      local.get 0
                                      i32.load offset=432
                                      local.get 8
                                      i32.add
                                      local.tee 1
                                      i32.store offset=432
                                      local.get 1
                                      local.get 0
                                      i32.load offset=436
                                      i32.gt_u
                                      if  ;; label = @18
                                        local.get 0
                                        i32.const 436
                                        i32.add
                                        local.get 1
                                        i32.store
                                      end
                                      block  ;; label = @18
                                        block  ;; label = @19
                                          block  ;; label = @20
                                            local.get 0
                                            i32.load offset=24
                                            local.tee 7
                                            if  ;; label = @21
                                              local.get 0
                                              i32.const 448
                                              i32.add
                                              local.tee 9
                                              local.set 1
                                              loop  ;; label = @22
                                                local.get 1
                                                i32.eqz
                                                br_if 3 (;@19;)
                                                local.get 3
                                                local.get 1
                                                i32.load
                                                local.tee 2
                                                local.get 1
                                                i32.load offset=4
                                                local.tee 4
                                                i32.add
                                                i32.eq
                                                br_if 2 (;@20;)
                                                local.get 1
                                                i32.load offset=8
                                                local.set 1
                                                br 0 (;@22;)
                                              end
                                              unreachable
                                            end
                                            block  ;; label = @21
                                              local.get 0
                                              i32.load offset=16
                                              local.tee 1
                                              if  ;; label = @22
                                                local.get 3
                                                local.get 1
                                                i32.ge_u
                                                br_if 1 (;@21;)
                                              end
                                              local.get 0
                                              i32.const 16
                                              i32.add
                                              local.get 3
                                              i32.store
                                            end
                                            local.get 0
                                            local.get 8
                                            i32.store offset=452
                                            local.get 0
                                            local.get 3
                                            i32.store offset=448
                                            i32.const 0
                                            local.set 1
                                            local.get 0
                                            i32.const 0
                                            i32.store offset=460
                                            local.get 0
                                            i32.const -1
                                            i32.store offset=32
                                            local.get 0
                                            i32.const 12
                                            i32.load
                                            i32.store offset=36
                                            block  ;; label = @21
                                              loop  ;; label = @22
                                                local.get 1
                                                i32.const 256
                                                i32.eq
                                                br_if 1 (;@21;)
                                                local.get 0
                                                local.get 1
                                                i32.add
                                                local.tee 2
                                                i32.const 48
                                                i32.add
                                                local.get 2
                                                i32.const 40
                                                i32.add
                                                local.tee 4
                                                i32.store
                                                local.get 2
                                                i32.const 52
                                                i32.add
                                                local.get 4
                                                i32.store
                                                local.get 1
                                                i32.const 8
                                                i32.add
                                                local.set 1
                                                br 0 (;@22;)
                                              end
                                              unreachable
                                            end
                                            local.get 0
                                            local.get 0
                                            local.get 0
                                            i32.const -4
                                            i32.add
                                            i32.load
                                            i32.const -8
                                            i32.and
                                            i32.add
                                            i32.const -8
                                            i32.add
                                            local.tee 1
                                            local.get 3
                                            local.get 8
                                            i32.add
                                            i32.const -40
                                            i32.add
                                            local.get 1
                                            i32.sub
                                            call $.init_top
                                            br 2 (;@18;)
                                          end
                                          local.get 1
                                          i32.load8_u offset=12
                                          i32.const 8
                                          i32.and
                                          local.get 3
                                          local.get 7
                                          i32.le_u
                                          i32.or
                                          local.get 2
                                          local.get 7
                                          i32.gt_u
                                          i32.or
                                          br_if 0 (;@19;)
                                          local.get 1
                                          i32.const 4
                                          i32.add
                                          local.get 4
                                          local.get 8
                                          i32.add
                                          i32.store
                                          local.get 0
                                          local.get 7
                                          local.get 0
                                          i32.const 12
                                          i32.add
                                          i32.load
                                          local.get 8
                                          i32.add
                                          call $.init_top
                                          br 1 (;@18;)
                                        end
                                        local.get 3
                                        local.get 0
                                        i32.load offset=16
                                        i32.lt_u
                                        if  ;; label = @19
                                          local.get 0
                                          i32.const 16
                                          i32.add
                                          local.get 3
                                          i32.store
                                        end
                                        local.get 3
                                        local.get 8
                                        i32.add
                                        local.set 2
                                        local.get 9
                                        local.set 1
                                        block (result i32)  ;; label = @19
                                          block  ;; label = @20
                                            block (result i32)  ;; label = @21
                                              block  ;; label = @22
                                                block  ;; label = @23
                                                  block  ;; label = @24
                                                    loop  ;; label = @25
                                                      local.get 1
                                                      i32.eqz
                                                      br_if 1 (;@24;)
                                                      local.get 1
                                                      i32.load
                                                      local.get 2
                                                      i32.ne
                                                      if  ;; label = @26
                                                        local.get 1
                                                        i32.load offset=8
                                                        local.set 1
                                                        br 1 (;@25;)
                                                      end
                                                    end
                                                    local.get 1
                                                    i32.load8_u offset=12
                                                    i32.const 8
                                                    i32.and
                                                    br_if 0 (;@24;)
                                                    local.get 1
                                                    local.get 3
                                                    i32.store
                                                    local.get 1
                                                    local.get 1
                                                    i32.load offset=4
                                                    local.get 8
                                                    i32.add
                                                    i32.store offset=4
                                                    local.get 3
                                                    i32.const -8
                                                    local.get 3
                                                    i32.sub
                                                    i32.const 7
                                                    i32.and
                                                    i32.const 0
                                                    local.get 3
                                                    i32.const 8
                                                    i32.add
                                                    i32.const 7
                                                    i32.and
                                                    select
                                                    i32.add
                                                    local.tee 8
                                                    local.get 5
                                                    i32.const 3
                                                    i32.or
                                                    i32.store offset=4
                                                    local.get 2
                                                    i32.const -8
                                                    local.get 2
                                                    i32.sub
                                                    i32.const 7
                                                    i32.and
                                                    i32.const 0
                                                    local.get 2
                                                    i32.const 8
                                                    i32.add
                                                    i32.const 7
                                                    i32.and
                                                    select
                                                    i32.add
                                                    local.tee 4
                                                    local.get 8
                                                    i32.sub
                                                    local.get 5
                                                    i32.sub
                                                    local.set 1
                                                    local.get 8
                                                    local.get 5
                                                    i32.add
                                                    local.set 5
                                                    local.get 7
                                                    local.get 4
                                                    i32.eq
                                                    br_if 1 (;@23;)
                                                    local.get 0
                                                    i32.load offset=20
                                                    local.get 4
                                                    i32.eq
                                                    br_if 8 (;@16;)
                                                    local.get 4
                                                    i32.load offset=4
                                                    local.tee 2
                                                    i32.const 3
                                                    i32.and
                                                    i32.const 1
                                                    i32.ne
                                                    br_if 16 (;@8;)
                                                    local.get 2
                                                    i32.const -8
                                                    i32.and
                                                    local.set 9
                                                    local.get 2
                                                    i32.const 255
                                                    i32.gt_u
                                                    br_if 9 (;@15;)
                                                    local.get 4
                                                    i32.load offset=12
                                                    local.tee 3
                                                    local.get 4
                                                    i32.load offset=8
                                                    local.tee 6
                                                    i32.eq
                                                    br_if 10 (;@14;)
                                                    local.get 3
                                                    local.get 6
                                                    i32.store offset=8
                                                    local.get 6
                                                    local.get 3
                                                    i32.store offset=12
                                                    br 15 (;@9;)
                                                  end
                                                  local.get 0
                                                  local.get 7
                                                  call $.segment_holding
                                                  local.tee 1
                                                  i32.load offset=4
                                                  local.set 2
                                                  local.get 1
                                                  i32.load
                                                  local.set 1
                                                  local.get 0
                                                  local.get 3
                                                  local.get 8
                                                  i32.const -40
                                                  i32.add
                                                  call $.init_top
                                                  local.get 7
                                                  local.get 1
                                                  local.get 2
                                                  i32.add
                                                  local.tee 2
                                                  i32.const 39
                                                  local.get 2
                                                  i32.sub
                                                  i32.const 7
                                                  i32.and
                                                  i32.const 0
                                                  local.get 2
                                                  i32.const -39
                                                  i32.add
                                                  i32.const 7
                                                  i32.and
                                                  select
                                                  i32.add
                                                  i32.const -47
                                                  i32.add
                                                  local.tee 1
                                                  local.get 1
                                                  local.get 7
                                                  i32.const 16
                                                  i32.add
                                                  i32.lt_u
                                                  select
                                                  local.tee 4
                                                  i32.const 27
                                                  i32.store offset=4
                                                  local.get 4
                                                  i32.const 16
                                                  i32.add
                                                  local.get 9
                                                  i32.const 8
                                                  i32.add
                                                  i64.load align=4
                                                  i64.store align=4
                                                  local.get 4
                                                  local.get 9
                                                  i64.load align=4
                                                  i64.store offset=8 align=4
                                                  local.get 0
                                                  i32.const 448
                                                  i32.add
                                                  local.get 3
                                                  i32.store
                                                  local.get 0
                                                  local.get 4
                                                  i32.const 8
                                                  i32.add
                                                  i32.store offset=456
                                                  local.get 0
                                                  i32.const 0
                                                  i32.store offset=460
                                                  local.get 0
                                                  local.get 8
                                                  i32.store offset=452
                                                  local.get 4
                                                  i32.const 28
                                                  i32.add
                                                  local.set 1
                                                  loop  ;; label = @24
                                                    local.get 1
                                                    i32.const 7
                                                    i32.store
                                                    local.get 1
                                                    i32.const 4
                                                    i32.add
                                                    local.tee 1
                                                    local.get 2
                                                    i32.lt_u
                                                    br_if 0 (;@24;)
                                                  end
                                                  local.get 4
                                                  local.get 7
                                                  i32.eq
                                                  br_if 5 (;@18;)
                                                  local.get 4
                                                  i32.const 4
                                                  i32.add
                                                  local.tee 1
                                                  local.get 1
                                                  i32.load
                                                  i32.const -2
                                                  i32.and
                                                  i32.store
                                                  local.get 4
                                                  local.get 4
                                                  local.get 7
                                                  i32.sub
                                                  local.tee 8
                                                  i32.store
                                                  local.get 7
                                                  local.get 8
                                                  i32.const 1
                                                  i32.or
                                                  i32.store offset=4
                                                  local.get 8
                                                  i32.const 255
                                                  i32.le_u
                                                  if  ;; label = @24
                                                    local.get 0
                                                    local.get 8
                                                    i32.const 3
                                                    i32.shr_u
                                                    local.tee 2
                                                    i32.const 3
                                                    i32.shl
                                                    i32.add
                                                    i32.const 40
                                                    i32.add
                                                    local.set 1
                                                    local.get 0
                                                    i32.load
                                                    local.tee 3
                                                    i32.const 1
                                                    local.get 2
                                                    i32.shl
                                                    local.tee 2
                                                    i32.and
                                                    i32.eqz
                                                    br_if 2 (;@22;)
                                                    local.get 1
                                                    i32.load offset=8
                                                    br 3 (;@21;)
                                                  end
                                                  local.get 8
                                                  i32.const 8
                                                  i32.shr_u
                                                  local.tee 2
                                                  i32.eqz
                                                  br_if 3 (;@20;)
                                                  i32.const 31
                                                  local.get 8
                                                  i32.const 16777215
                                                  i32.gt_u
                                                  br_if 4 (;@19;)
                                                  drop
                                                  local.get 8
                                                  i32.const 14
                                                  local.get 2
                                                  local.get 2
                                                  i32.const 1048320
                                                  i32.add
                                                  i32.const 16
                                                  i32.shr_u
                                                  i32.const 8
                                                  i32.and
                                                  local.tee 1
                                                  i32.shl
                                                  local.tee 2
                                                  i32.const 520192
                                                  i32.add
                                                  i32.const 16
                                                  i32.shr_u
                                                  i32.const 4
                                                  i32.and
                                                  local.tee 3
                                                  local.get 1
                                                  i32.or
                                                  local.get 2
                                                  local.get 3
                                                  i32.shl
                                                  local.tee 1
                                                  i32.const 245760
                                                  i32.add
                                                  i32.const 16
                                                  i32.shr_u
                                                  i32.const 2
                                                  i32.and
                                                  local.tee 2
                                                  i32.or
                                                  i32.sub
                                                  local.get 1
                                                  local.get 2
                                                  i32.shl
                                                  i32.const 15
                                                  i32.shr_u
                                                  i32.add
                                                  local.tee 1
                                                  i32.const 7
                                                  i32.add
                                                  i32.shr_u
                                                  i32.const 1
                                                  i32.and
                                                  local.get 1
                                                  i32.const 1
                                                  i32.shl
                                                  i32.or
                                                  br 4 (;@19;)
                                                end
                                                local.get 0
                                                i32.const 24
                                                i32.add
                                                local.get 5
                                                i32.store
                                                local.get 0
                                                i32.const 12
                                                i32.add
                                                local.tee 2
                                                local.get 2
                                                i32.load
                                                local.get 1
                                                i32.add
                                                local.tee 1
                                                i32.store
                                                local.get 5
                                                local.get 1
                                                i32.const 1
                                                i32.or
                                                i32.store offset=4
                                                br 15 (;@7;)
                                              end
                                              local.get 0
                                              local.get 3
                                              local.get 2
                                              i32.or
                                              i32.store
                                              local.get 1
                                            end
                                            local.tee 2
                                            local.get 7
                                            i32.store offset=12
                                            local.get 1
                                            i32.const 8
                                            i32.add
                                            local.get 7
                                            i32.store
                                            local.get 7
                                            local.get 1
                                            i32.store offset=12
                                            local.get 7
                                            local.get 2
                                            i32.store offset=8
                                            br 2 (;@18;)
                                          end
                                          i32.const 0
                                        end
                                        local.set 1
                                        local.get 7
                                        i64.const 0
                                        i64.store offset=16 align=4
                                        local.get 7
                                        i32.const 28
                                        i32.add
                                        local.get 1
                                        i32.store
                                        local.get 0
                                        local.get 1
                                        i32.const 2
                                        i32.shl
                                        i32.add
                                        i32.const 304
                                        i32.add
                                        local.set 2
                                        block  ;; label = @19
                                          block  ;; label = @20
                                            local.get 0
                                            i32.load offset=4
                                            local.tee 3
                                            i32.const 1
                                            local.get 1
                                            i32.shl
                                            local.tee 4
                                            i32.and
                                            if  ;; label = @21
                                              local.get 8
                                              i32.const 0
                                              i32.const 25
                                              local.get 1
                                              i32.const 1
                                              i32.shr_u
                                              i32.sub
                                              local.get 1
                                              i32.const 31
                                              i32.eq
                                              select
                                              i32.shl
                                              local.set 1
                                              local.get 2
                                              i32.load
                                              local.set 3
                                              loop  ;; label = @22
                                                local.get 3
                                                local.tee 2
                                                i32.load offset=4
                                                i32.const -8
                                                i32.and
                                                local.get 8
                                                i32.eq
                                                br_if 3 (;@19;)
                                                local.get 1
                                                i32.const 29
                                                i32.shr_u
                                                local.set 3
                                                local.get 1
                                                i32.const 1
                                                i32.shl
                                                local.set 1
                                                local.get 2
                                                local.get 3
                                                i32.const 4
                                                i32.and
                                                i32.add
                                                i32.const 16
                                                i32.add
                                                local.tee 4
                                                i32.load
                                                local.tee 3
                                                br_if 0 (;@22;)
                                              end
                                              local.get 4
                                              local.get 7
                                              i32.store
                                              local.get 7
                                              i32.const 24
                                              i32.add
                                              local.get 2
                                              i32.store
                                              br 1 (;@20;)
                                            end
                                            local.get 0
                                            i32.const 4
                                            i32.add
                                            local.get 3
                                            local.get 4
                                            i32.or
                                            i32.store
                                            local.get 2
                                            local.get 7
                                            i32.store
                                            local.get 7
                                            i32.const 24
                                            i32.add
                                            local.get 2
                                            i32.store
                                          end
                                          local.get 7
                                          local.get 7
                                          i32.store offset=12
                                          local.get 7
                                          local.get 7
                                          i32.store offset=8
                                          br 1 (;@18;)
                                        end
                                        local.get 2
                                        i32.load offset=8
                                        local.tee 1
                                        local.get 7
                                        i32.store offset=12
                                        local.get 2
                                        local.get 7
                                        i32.store offset=8
                                        local.get 7
                                        i32.const 24
                                        i32.add
                                        i32.const 0
                                        i32.store
                                        local.get 7
                                        local.get 2
                                        i32.store offset=12
                                        local.get 7
                                        local.get 1
                                        i32.store offset=8
                                      end
                                      local.get 0
                                      i32.const 12
                                      i32.add
                                      local.tee 1
                                      i32.load
                                      local.tee 2
                                      local.get 5
                                      i32.le_u
                                      br_if 0 (;@17;)
                                      local.get 0
                                      i32.const 24
                                      i32.add
                                      local.tee 4
                                      i32.load
                                      local.tee 3
                                      local.get 5
                                      i32.add
                                      local.tee 6
                                      local.get 2
                                      local.get 5
                                      i32.sub
                                      local.tee 2
                                      i32.const 1
                                      i32.or
                                      i32.store offset=4
                                      local.get 1
                                      local.get 2
                                      i32.store
                                      local.get 4
                                      local.get 6
                                      i32.store
                                      local.get 3
                                      local.get 5
                                      i32.const 3
                                      i32.or
                                      i32.store offset=4
                                      local.get 3
                                      i32.const 8
                                      i32.add
                                      local.set 6
                                    end
                                    local.get 6
                                    return
                                  end
                                  local.get 5
                                  local.get 0
                                  i32.const 8
                                  i32.add
                                  local.tee 2
                                  i32.load
                                  local.get 1
                                  i32.add
                                  local.tee 1
                                  i32.const 1
                                  i32.or
                                  i32.store offset=4
                                  local.get 0
                                  i32.const 20
                                  i32.add
                                  local.get 5
                                  i32.store
                                  local.get 2
                                  local.get 1
                                  i32.store
                                  local.get 5
                                  local.get 1
                                  i32.add
                                  local.get 1
                                  i32.store
                                  br 8 (;@7;)
                                end
                                local.get 4
                                i32.load offset=24
                                local.set 10
                                local.get 4
                                i32.load offset=12
                                local.tee 6
                                local.get 4
                                i32.eq
                                br_if 1 (;@13;)
                                local.get 4
                                i32.load offset=8
                                local.tee 2
                                local.get 6
                                i32.store offset=12
                                local.get 6
                                local.get 2
                                i32.store offset=8
                                local.get 10
                                br_if 4 (;@10;)
                                br 5 (;@9;)
                              end
                              local.get 0
                              local.get 0
                              i32.load
                              i32.const -2
                              local.get 2
                              i32.const 3
                              i32.shr_u
                              i32.rotl
                              i32.and
                              i32.store
                              br 4 (;@9;)
                            end
                            local.get 4
                            i32.const 20
                            i32.add
                            local.tee 2
                            i32.load
                            local.tee 3
                            i32.eqz
                            if  ;; label = @13
                              local.get 4
                              i32.const 16
                              i32.add
                              local.tee 2
                              i32.load
                              local.tee 3
                              i32.eqz
                              br_if 2 (;@11;)
                            end
                            loop  ;; label = @13
                              local.get 2
                              local.set 7
                              local.get 3
                              local.tee 6
                              i32.const 20
                              i32.add
                              local.tee 2
                              i32.load
                              local.tee 3
                              br_if 0 (;@13;)
                              local.get 6
                              i32.const 16
                              i32.add
                              local.set 2
                              local.get 6
                              i32.load offset=16
                              local.tee 3
                              br_if 0 (;@13;)
                            end
                            local.get 7
                            i32.const 0
                            i32.store
                            local.get 10
                            i32.eqz
                            br_if 3 (;@9;)
                            br 2 (;@10;)
                          end
                          i32.const 0
                          return
                        end
                        i32.const 0
                        local.set 6
                        local.get 10
                        i32.eqz
                        br_if 1 (;@9;)
                      end
                      block  ;; label = @10
                        block  ;; label = @11
                          local.get 0
                          local.get 4
                          i32.load offset=28
                          local.tee 3
                          i32.const 2
                          i32.shl
                          i32.add
                          i32.const 304
                          i32.add
                          local.tee 2
                          i32.load
                          local.get 4
                          i32.ne
                          if  ;; label = @12
                            local.get 10
                            i32.const 16
                            i32.add
                            local.get 10
                            i32.load offset=16
                            local.get 4
                            i32.ne
                            i32.const 2
                            i32.shl
                            i32.add
                            local.get 6
                            i32.store
                            local.get 6
                            br_if 1 (;@11;)
                            br 3 (;@9;)
                          end
                          local.get 2
                          local.get 6
                          i32.store
                          local.get 6
                          i32.eqz
                          br_if 1 (;@10;)
                        end
                        local.get 6
                        local.get 10
                        i32.store offset=24
                        local.get 4
                        i32.load offset=16
                        local.tee 2
                        if  ;; label = @11
                          local.get 6
                          local.get 2
                          i32.store offset=16
                          local.get 2
                          local.get 6
                          i32.store offset=24
                        end
                        local.get 4
                        i32.const 20
                        i32.add
                        i32.load
                        local.tee 2
                        i32.eqz
                        br_if 1 (;@9;)
                        local.get 6
                        i32.const 20
                        i32.add
                        local.get 2
                        i32.store
                        local.get 2
                        local.get 6
                        i32.store offset=24
                        br 1 (;@9;)
                      end
                      local.get 0
                      local.get 0
                      i32.load offset=4
                      i32.const -2
                      local.get 3
                      i32.rotl
                      i32.and
                      i32.store offset=4
                    end
                    local.get 9
                    local.get 1
                    i32.add
                    local.set 1
                    local.get 4
                    local.get 9
                    i32.add
                    local.set 4
                  end
                  local.get 4
                  local.get 4
                  i32.load offset=4
                  i32.const -2
                  i32.and
                  i32.store offset=4
                  local.get 5
                  local.get 1
                  i32.const 1
                  i32.or
                  i32.store offset=4
                  local.get 5
                  local.get 1
                  i32.add
                  local.get 1
                  i32.store
                  block (result i32)  ;; label = @8
                    block  ;; label = @9
                      block (result i32)  ;; label = @10
                        block  ;; label = @11
                          local.get 1
                          i32.const 255
                          i32.le_u
                          if  ;; label = @12
                            local.get 0
                            local.get 1
                            i32.const 3
                            i32.shr_u
                            local.tee 2
                            i32.const 3
                            i32.shl
                            i32.add
                            i32.const 40
                            i32.add
                            local.set 1
                            local.get 0
                            i32.load
                            local.tee 3
                            i32.const 1
                            local.get 2
                            i32.shl
                            local.tee 2
                            i32.and
                            i32.eqz
                            br_if 1 (;@11;)
                            local.get 1
                            i32.load offset=8
                            local.set 2
                            local.get 1
                            i32.const 8
                            i32.add
                            br 2 (;@10;)
                          end
                          local.get 1
                          i32.const 8
                          i32.shr_u
                          local.tee 3
                          i32.eqz
                          br_if 2 (;@9;)
                          i32.const 31
                          local.get 1
                          i32.const 16777215
                          i32.gt_u
                          br_if 3 (;@8;)
                          drop
                          local.get 1
                          i32.const 14
                          local.get 3
                          local.get 3
                          i32.const 1048320
                          i32.add
                          i32.const 16
                          i32.shr_u
                          i32.const 8
                          i32.and
                          local.tee 2
                          i32.shl
                          local.tee 3
                          i32.const 520192
                          i32.add
                          i32.const 16
                          i32.shr_u
                          i32.const 4
                          i32.and
                          local.tee 4
                          local.get 2
                          i32.or
                          local.get 3
                          local.get 4
                          i32.shl
                          local.tee 2
                          i32.const 245760
                          i32.add
                          i32.const 16
                          i32.shr_u
                          i32.const 2
                          i32.and
                          local.tee 3
                          i32.or
                          i32.sub
                          local.get 2
                          local.get 3
                          i32.shl
                          i32.const 15
                          i32.shr_u
                          i32.add
                          local.tee 2
                          i32.const 7
                          i32.add
                          i32.shr_u
                          i32.const 1
                          i32.and
                          local.get 2
                          i32.const 1
                          i32.shl
                          i32.or
                          br 3 (;@8;)
                        end
                        local.get 0
                        local.get 3
                        local.get 2
                        i32.or
                        i32.store
                        local.get 1
                        local.set 2
                        local.get 1
                        i32.const 8
                        i32.add
                      end
                      local.set 3
                      local.get 2
                      local.get 5
                      i32.store offset=12
                      local.get 3
                      local.get 5
                      i32.store
                      local.get 5
                      local.get 1
                      i32.store offset=12
                      local.get 5
                      local.get 2
                      i32.store offset=8
                      br 2 (;@7;)
                    end
                    i32.const 0
                  end
                  local.set 2
                  local.get 5
                  local.get 2
                  i32.store offset=28
                  local.get 5
                  i64.const 0
                  i64.store offset=16 align=4
                  local.get 0
                  local.get 2
                  i32.const 2
                  i32.shl
                  i32.add
                  i32.const 304
                  i32.add
                  local.set 3
                  block  ;; label = @8
                    block  ;; label = @9
                      local.get 0
                      i32.load offset=4
                      local.tee 4
                      i32.const 1
                      local.get 2
                      i32.shl
                      local.tee 6
                      i32.and
                      if  ;; label = @10
                        local.get 1
                        i32.const 0
                        i32.const 25
                        local.get 2
                        i32.const 1
                        i32.shr_u
                        i32.sub
                        local.get 2
                        i32.const 31
                        i32.eq
                        select
                        i32.shl
                        local.set 2
                        local.get 3
                        i32.load
                        local.set 4
                        loop  ;; label = @11
                          local.get 4
                          local.tee 3
                          i32.load offset=4
                          i32.const -8
                          i32.and
                          local.get 1
                          i32.eq
                          br_if 3 (;@8;)
                          local.get 2
                          i32.const 29
                          i32.shr_u
                          local.set 4
                          local.get 2
                          i32.const 1
                          i32.shl
                          local.set 2
                          local.get 3
                          local.get 4
                          i32.const 4
                          i32.and
                          i32.add
                          i32.const 16
                          i32.add
                          local.tee 6
                          i32.load
                          local.tee 4
                          br_if 0 (;@11;)
                        end
                        local.get 6
                        local.get 5
                        i32.store
                        local.get 5
                        local.get 3
                        i32.store offset=24
                        br 1 (;@9;)
                      end
                      local.get 0
                      i32.const 4
                      i32.add
                      local.get 4
                      local.get 6
                      i32.or
                      i32.store
                      local.get 3
                      local.get 5
                      i32.store
                      local.get 5
                      local.get 3
                      i32.store offset=24
                    end
                    local.get 5
                    local.get 5
                    i32.store offset=12
                    local.get 5
                    local.get 5
                    i32.store offset=8
                    br 1 (;@7;)
                  end
                  local.get 3
                  i32.load offset=8
                  local.tee 1
                  local.get 5
                  i32.store offset=12
                  local.get 3
                  local.get 5
                  i32.store offset=8
                  local.get 5
                  i32.const 0
                  i32.store offset=24
                  local.get 5
                  local.get 3
                  i32.store offset=12
                  local.get 5
                  local.get 1
                  i32.store offset=8
                end
                local.get 8
                i32.const 8
                i32.add
                return
              end
              block  ;; label = @6
                block  ;; label = @7
                  local.get 3
                  local.get 0
                  local.get 3
                  i32.load offset=28
                  local.tee 4
                  i32.const 2
                  i32.shl
                  i32.add
                  i32.const 304
                  i32.add
                  local.tee 1
                  i32.load
                  i32.ne
                  if  ;; label = @8
                    local.get 7
                    i32.const 16
                    i32.add
                    local.get 7
                    i32.load offset=16
                    local.get 3
                    i32.ne
                    i32.const 2
                    i32.shl
                    i32.add
                    local.get 6
                    i32.store
                    local.get 6
                    br_if 1 (;@7;)
                    br 3 (;@5;)
                  end
                  local.get 1
                  local.get 6
                  i32.store
                  local.get 6
                  i32.eqz
                  br_if 1 (;@6;)
                end
                local.get 6
                local.get 7
                i32.store offset=24
                local.get 3
                i32.load offset=16
                local.tee 1
                if  ;; label = @7
                  local.get 6
                  local.get 1
                  i32.store offset=16
                  local.get 1
                  local.get 6
                  i32.store offset=24
                end
                local.get 3
                i32.const 20
                i32.add
                i32.load
                local.tee 1
                i32.eqz
                br_if 1 (;@5;)
                local.get 6
                i32.const 20
                i32.add
                local.get 1
                i32.store
                local.get 1
                local.get 6
                i32.store offset=24
                br 1 (;@5;)
              end
              local.get 0
              i32.const 4
              i32.add
              local.tee 1
              local.get 1
              i32.load
              i32.const -2
              local.get 4
              i32.rotl
              i32.and
              i32.store
            end
            block  ;; label = @5
              local.get 2
              i32.const 15
              i32.le_u
              if  ;; label = @6
                local.get 3
                local.get 2
                local.get 5
                i32.add
                local.tee 1
                i32.const 3
                i32.or
                i32.store offset=4
                local.get 3
                local.get 1
                i32.add
                local.tee 1
                local.get 1
                i32.load offset=4
                i32.const 1
                i32.or
                i32.store offset=4
                br 1 (;@5;)
              end
              local.get 3
              local.get 5
              i32.const 3
              i32.or
              i32.store offset=4
              local.get 9
              local.get 2
              i32.const 1
              i32.or
              i32.store offset=4
              local.get 9
              local.get 2
              i32.add
              local.get 2
              i32.store
              local.get 0
              i32.const 8
              i32.add
              local.tee 4
              i32.load
              local.tee 1
              if  ;; label = @6
                local.get 0
                local.get 1
                i32.const 3
                i32.shr_u
                local.tee 6
                i32.const 3
                i32.shl
                i32.add
                i32.const 40
                i32.add
                local.set 5
                local.get 0
                i32.const 20
                i32.add
                i32.load
                local.set 1
                block (result i32)  ;; label = @7
                  local.get 5
                  i32.load offset=8
                  local.get 0
                  i32.load
                  local.tee 8
                  i32.const 1
                  local.get 6
                  i32.shl
                  local.tee 6
                  i32.and
                  br_if 0 (;@7;)
                  drop
                  local.get 0
                  local.get 8
                  local.get 6
                  i32.or
                  i32.store
                  local.get 5
                end
                local.tee 6
                local.get 1
                i32.store offset=12
                local.get 5
                i32.const 8
                i32.add
                local.get 1
                i32.store
                local.get 1
                local.get 5
                i32.store offset=12
                local.get 1
                local.get 6
                i32.store offset=8
              end
              local.get 0
              i32.const 20
              i32.add
              local.get 9
              i32.store
              local.get 4
              local.get 2
              i32.store
            end
            local.get 3
            i32.const 8
            i32.add
            return
          end
          block  ;; label = @4
            block  ;; label = @5
              local.get 3
              local.get 0
              local.get 3
              i32.load offset=28
              local.tee 4
              i32.const 2
              i32.shl
              i32.add
              i32.const 304
              i32.add
              local.tee 1
              i32.load
              i32.ne
              if  ;; label = @6
                local.get 9
                i32.const 16
                i32.add
                local.get 9
                i32.load offset=16
                local.get 3
                i32.ne
                i32.const 2
                i32.shl
                i32.add
                local.get 6
                i32.store
                local.get 6
                br_if 1 (;@5;)
                br 3 (;@3;)
              end
              local.get 1
              local.get 6
              i32.store
              local.get 6
              i32.eqz
              br_if 1 (;@4;)
            end
            local.get 6
            local.get 9
            i32.store offset=24
            local.get 3
            i32.load offset=16
            local.tee 1
            if  ;; label = @5
              local.get 6
              local.get 1
              i32.store offset=16
              local.get 1
              local.get 6
              i32.store offset=24
            end
            local.get 3
            i32.const 20
            i32.add
            i32.load
            local.tee 1
            i32.eqz
            br_if 1 (;@3;)
            local.get 6
            i32.const 20
            i32.add
            local.get 1
            i32.store
            local.get 1
            local.get 6
            i32.store offset=24
            br 1 (;@3;)
          end
          local.get 0
          i32.const 4
          i32.add
          local.tee 1
          local.get 1
          i32.load
          i32.const -2
          local.get 4
          i32.rotl
          i32.and
          i32.store
        end
        block  ;; label = @3
          local.get 2
          i32.const 15
          i32.le_u
          if  ;; label = @4
            local.get 3
            local.get 2
            local.get 5
            i32.add
            local.tee 1
            i32.const 3
            i32.or
            i32.store offset=4
            local.get 3
            local.get 1
            i32.add
            local.tee 1
            local.get 1
            i32.load offset=4
            i32.const 1
            i32.or
            i32.store offset=4
            br 1 (;@3;)
          end
          local.get 3
          local.get 5
          i32.const 3
          i32.or
          i32.store offset=4
          local.get 7
          local.get 2
          i32.const 1
          i32.or
          i32.store offset=4
          local.get 7
          local.get 2
          i32.add
          local.get 2
          i32.store
          block (result i32)  ;; label = @4
            block  ;; label = @5
              block (result i32)  ;; label = @6
                block  ;; label = @7
                  local.get 2
                  i32.const 255
                  i32.le_u
                  if  ;; label = @8
                    local.get 0
                    local.get 2
                    i32.const 3
                    i32.shr_u
                    local.tee 2
                    i32.const 3
                    i32.shl
                    i32.add
                    i32.const 40
                    i32.add
                    local.set 1
                    local.get 0
                    i32.load
                    local.tee 5
                    i32.const 1
                    local.get 2
                    i32.shl
                    local.tee 2
                    i32.and
                    i32.eqz
                    br_if 1 (;@7;)
                    local.get 1
                    i32.const 8
                    i32.add
                    local.set 5
                    local.get 1
                    i32.load offset=8
                    br 2 (;@6;)
                  end
                  local.get 2
                  i32.const 8
                  i32.shr_u
                  local.tee 5
                  i32.eqz
                  br_if 2 (;@5;)
                  i32.const 31
                  local.get 2
                  i32.const 16777215
                  i32.gt_u
                  br_if 3 (;@4;)
                  drop
                  local.get 2
                  i32.const 14
                  local.get 5
                  local.get 5
                  i32.const 1048320
                  i32.add
                  i32.const 16
                  i32.shr_u
                  i32.const 8
                  i32.and
                  local.tee 1
                  i32.shl
                  local.tee 5
                  i32.const 520192
                  i32.add
                  i32.const 16
                  i32.shr_u
                  i32.const 4
                  i32.and
                  local.tee 4
                  local.get 1
                  i32.or
                  local.get 5
                  local.get 4
                  i32.shl
                  local.tee 1
                  i32.const 245760
                  i32.add
                  i32.const 16
                  i32.shr_u
                  i32.const 2
                  i32.and
                  local.tee 5
                  i32.or
                  i32.sub
                  local.get 1
                  local.get 5
                  i32.shl
                  i32.const 15
                  i32.shr_u
                  i32.add
                  local.tee 1
                  i32.const 7
                  i32.add
                  i32.shr_u
                  i32.const 1
                  i32.and
                  local.get 1
                  i32.const 1
                  i32.shl
                  i32.or
                  br 3 (;@4;)
                end
                local.get 0
                local.get 5
                local.get 2
                i32.or
                i32.store
                local.get 1
                i32.const 8
                i32.add
                local.set 5
                local.get 1
              end
              local.tee 2
              local.get 7
              i32.store offset=12
              local.get 5
              local.get 7
              i32.store
              local.get 7
              local.get 1
              i32.store offset=12
              local.get 7
              local.get 2
              i32.store offset=8
              br 2 (;@3;)
            end
            i32.const 0
          end
          local.set 1
          local.get 7
          local.get 1
          i32.store offset=28
          local.get 7
          i64.const 0
          i64.store offset=16 align=4
          local.get 0
          local.get 1
          i32.const 2
          i32.shl
          i32.add
          i32.const 304
          i32.add
          local.set 5
          block  ;; label = @4
            block  ;; label = @5
              local.get 0
              i32.const 4
              i32.add
              local.tee 4
              i32.load
              local.tee 6
              i32.const 1
              local.get 1
              i32.shl
              local.tee 8
              i32.and
              if  ;; label = @6
                local.get 2
                i32.const 0
                i32.const 25
                local.get 1
                i32.const 1
                i32.shr_u
                i32.sub
                local.get 1
                i32.const 31
                i32.eq
                select
                i32.shl
                local.set 1
                local.get 5
                i32.load
                local.set 4
                loop  ;; label = @7
                  local.get 4
                  local.tee 5
                  i32.load offset=4
                  i32.const -8
                  i32.and
                  local.get 2
                  i32.eq
                  br_if 3 (;@4;)
                  local.get 1
                  i32.const 29
                  i32.shr_u
                  local.set 4
                  local.get 1
                  i32.const 1
                  i32.shl
                  local.set 1
                  local.get 5
                  local.get 4
                  i32.const 4
                  i32.and
                  i32.add
                  i32.const 16
                  i32.add
                  local.tee 6
                  i32.load
                  local.tee 4
                  br_if 0 (;@7;)
                end
                local.get 6
                local.get 7
                i32.store
                local.get 7
                local.get 5
                i32.store offset=24
                br 1 (;@5;)
              end
              local.get 4
              local.get 6
              local.get 8
              i32.or
              i32.store
              local.get 5
              local.get 7
              i32.store
              local.get 7
              local.get 5
              i32.store offset=24
            end
            local.get 7
            local.get 7
            i32.store offset=12
            local.get 7
            local.get 7
            i32.store offset=8
            br 1 (;@3;)
          end
          local.get 5
          i32.load offset=8
          local.tee 1
          local.get 7
          i32.store offset=12
          local.get 5
          local.get 7
          i32.store offset=8
          local.get 7
          i32.const 0
          i32.store offset=24
          local.get 7
          local.get 5
          i32.store offset=12
          local.get 7
          local.get 1
          i32.store offset=8
        end
        local.get 3
        i32.const 8
        i32.add
      end
    end)
  (func $.mspace_free (type 4) (param i32 i32)
    (local i32 i32 i32 i32 i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        i32.eqz
        br_if 0 (;@2;)
        local.get 1
        i32.const -8
        i32.add
        local.tee 2
        local.get 0
        i32.load offset=16
        local.tee 4
        i32.lt_u
        br_if 0 (;@2;)
        local.get 1
        i32.const -4
        i32.add
        i32.load
        local.tee 1
        i32.const 3
        i32.and
        local.tee 3
        i32.const 1
        i32.eq
        br_if 0 (;@2;)
        local.get 2
        local.get 1
        i32.const -8
        i32.and
        local.tee 5
        i32.add
        local.set 6
        block  ;; label = @3
          local.get 1
          i32.const 1
          i32.and
          br_if 0 (;@3;)
          local.get 3
          i32.eqz
          br_if 1 (;@2;)
          local.get 2
          local.get 2
          i32.load
          local.tee 1
          i32.sub
          local.tee 2
          local.get 4
          i32.lt_u
          br_if 1 (;@2;)
          local.get 1
          local.get 5
          i32.add
          local.set 5
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                block  ;; label = @7
                  local.get 0
                  i32.load offset=20
                  local.get 2
                  i32.ne
                  if  ;; label = @8
                    local.get 1
                    i32.const 255
                    i32.gt_u
                    br_if 1 (;@7;)
                    local.get 2
                    i32.load offset=12
                    local.tee 4
                    local.get 2
                    i32.load offset=8
                    local.tee 3
                    i32.eq
                    br_if 2 (;@6;)
                    local.get 4
                    local.get 3
                    i32.store offset=8
                    local.get 3
                    local.get 4
                    i32.store offset=12
                    br 5 (;@3;)
                  end
                  local.get 6
                  i32.load offset=4
                  local.tee 1
                  i32.const 3
                  i32.and
                  i32.const 3
                  i32.ne
                  br_if 4 (;@3;)
                  local.get 6
                  i32.const 4
                  i32.add
                  local.get 1
                  i32.const -2
                  i32.and
                  i32.store
                  local.get 2
                  local.get 5
                  i32.const 1
                  i32.or
                  i32.store offset=4
                  local.get 0
                  local.get 5
                  i32.store offset=8
                  local.get 2
                  local.get 5
                  i32.add
                  local.get 5
                  i32.store
                  return
                end
                local.get 2
                i32.load offset=24
                local.set 7
                local.get 2
                i32.load offset=12
                local.tee 3
                local.get 2
                i32.eq
                br_if 1 (;@5;)
                local.get 2
                i32.load offset=8
                local.tee 1
                local.get 3
                i32.store offset=12
                local.get 3
                local.get 1
                i32.store offset=8
                local.get 7
                br_if 2 (;@4;)
                br 3 (;@3;)
              end
              local.get 0
              local.get 0
              i32.load
              i32.const -2
              local.get 1
              i32.const 3
              i32.shr_u
              i32.rotl
              i32.and
              i32.store
              br 2 (;@3;)
            end
            block  ;; label = @5
              local.get 2
              i32.const 20
              i32.add
              local.tee 1
              i32.load
              local.tee 4
              i32.eqz
              if  ;; label = @6
                local.get 2
                i32.const 16
                i32.add
                local.tee 1
                i32.load
                local.tee 4
                i32.eqz
                br_if 1 (;@5;)
              end
              loop  ;; label = @6
                local.get 1
                local.set 8
                local.get 4
                local.tee 3
                i32.const 20
                i32.add
                local.tee 1
                i32.load
                local.tee 4
                br_if 0 (;@6;)
                local.get 3
                i32.const 16
                i32.add
                local.set 1
                local.get 3
                i32.load offset=16
                local.tee 4
                br_if 0 (;@6;)
              end
              local.get 8
              i32.const 0
              i32.store
              local.get 7
              i32.eqz
              br_if 2 (;@3;)
              br 1 (;@4;)
            end
            i32.const 0
            local.set 3
            local.get 7
            i32.eqz
            br_if 1 (;@3;)
          end
          block  ;; label = @4
            block  ;; label = @5
              local.get 0
              local.get 2
              i32.load offset=28
              local.tee 4
              i32.const 2
              i32.shl
              i32.add
              i32.const 304
              i32.add
              local.tee 1
              i32.load
              local.get 2
              i32.ne
              if  ;; label = @6
                local.get 7
                i32.const 16
                i32.add
                local.get 7
                i32.load offset=16
                local.get 2
                i32.ne
                i32.const 2
                i32.shl
                i32.add
                local.get 3
                i32.store
                local.get 3
                br_if 1 (;@5;)
                br 3 (;@3;)
              end
              local.get 1
              local.get 3
              i32.store
              local.get 3
              i32.eqz
              br_if 1 (;@4;)
            end
            local.get 3
            local.get 7
            i32.store offset=24
            local.get 2
            i32.load offset=16
            local.tee 1
            if  ;; label = @5
              local.get 3
              local.get 1
              i32.store offset=16
              local.get 1
              local.get 3
              i32.store offset=24
            end
            local.get 2
            i32.const 20
            i32.add
            i32.load
            local.tee 1
            i32.eqz
            br_if 1 (;@3;)
            local.get 3
            i32.const 20
            i32.add
            local.get 1
            i32.store
            local.get 1
            local.get 3
            i32.store offset=24
            br 1 (;@3;)
          end
          local.get 0
          local.get 0
          i32.load offset=4
          i32.const -2
          local.get 4
          i32.rotl
          i32.and
          i32.store offset=4
        end
        local.get 2
        local.get 6
        i32.ge_u
        br_if 0 (;@2;)
        local.get 6
        i32.load offset=4
        local.tee 1
        i32.const 1
        i32.and
        i32.eqz
        br_if 0 (;@2;)
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                block  ;; label = @7
                  block  ;; label = @8
                    block  ;; label = @9
                      block  ;; label = @10
                        local.get 1
                        i32.const 2
                        i32.and
                        i32.eqz
                        if  ;; label = @11
                          local.get 0
                          i32.load offset=24
                          local.get 6
                          i32.eq
                          br_if 1 (;@10;)
                          local.get 0
                          i32.load offset=20
                          local.get 6
                          i32.eq
                          br_if 2 (;@9;)
                          local.get 1
                          i32.const -8
                          i32.and
                          local.get 5
                          i32.add
                          local.set 5
                          local.get 1
                          i32.const 255
                          i32.gt_u
                          br_if 3 (;@8;)
                          local.get 6
                          i32.load offset=12
                          local.tee 4
                          local.get 6
                          i32.load offset=8
                          local.tee 3
                          i32.eq
                          br_if 4 (;@7;)
                          local.get 4
                          local.get 3
                          i32.store offset=8
                          local.get 3
                          local.get 4
                          i32.store offset=12
                          br 7 (;@4;)
                        end
                        local.get 6
                        i32.const 4
                        i32.add
                        local.get 1
                        i32.const -2
                        i32.and
                        i32.store
                        local.get 2
                        local.get 5
                        i32.add
                        local.get 5
                        i32.store
                        local.get 2
                        local.get 5
                        i32.const 1
                        i32.or
                        i32.store offset=4
                        br 7 (;@3;)
                      end
                      local.get 0
                      i32.const 24
                      i32.add
                      local.get 2
                      i32.store
                      local.get 0
                      local.get 0
                      i32.load offset=12
                      local.get 5
                      i32.add
                      local.tee 1
                      i32.store offset=12
                      local.get 2
                      local.get 1
                      i32.const 1
                      i32.or
                      i32.store offset=4
                      local.get 2
                      local.get 0
                      i32.load offset=20
                      i32.ne
                      br_if 7 (;@2;)
                      local.get 0
                      i32.const 0
                      i32.store offset=8
                      local.get 0
                      i32.const 20
                      i32.add
                      i32.const 0
                      i32.store
                      return
                    end
                    local.get 0
                    i32.const 20
                    i32.add
                    local.get 2
                    i32.store
                    local.get 0
                    local.get 0
                    i32.load offset=8
                    local.get 5
                    i32.add
                    local.tee 1
                    i32.store offset=8
                    local.get 2
                    local.get 1
                    i32.const 1
                    i32.or
                    i32.store offset=4
                    local.get 2
                    local.get 1
                    i32.add
                    local.get 1
                    i32.store
                    return
                  end
                  local.get 6
                  i32.load offset=24
                  local.set 7
                  local.get 6
                  i32.load offset=12
                  local.tee 3
                  local.get 6
                  i32.eq
                  br_if 1 (;@6;)
                  local.get 6
                  i32.load offset=8
                  local.tee 1
                  local.get 3
                  i32.store offset=12
                  local.get 3
                  local.get 1
                  i32.store offset=8
                  local.get 7
                  br_if 2 (;@5;)
                  br 3 (;@4;)
                end
                local.get 0
                local.get 0
                i32.load
                i32.const -2
                local.get 1
                i32.const 3
                i32.shr_u
                i32.rotl
                i32.and
                i32.store
                br 2 (;@4;)
              end
              block  ;; label = @6
                local.get 6
                i32.const 20
                i32.add
                local.tee 1
                i32.load
                local.tee 4
                i32.eqz
                if  ;; label = @7
                  local.get 6
                  i32.const 16
                  i32.add
                  local.tee 1
                  i32.load
                  local.tee 4
                  i32.eqz
                  br_if 1 (;@6;)
                end
                loop  ;; label = @7
                  local.get 1
                  local.set 8
                  local.get 4
                  local.tee 3
                  i32.const 20
                  i32.add
                  local.tee 1
                  i32.load
                  local.tee 4
                  br_if 0 (;@7;)
                  local.get 3
                  i32.const 16
                  i32.add
                  local.set 1
                  local.get 3
                  i32.load offset=16
                  local.tee 4
                  br_if 0 (;@7;)
                end
                local.get 8
                i32.const 0
                i32.store
                local.get 7
                i32.eqz
                br_if 2 (;@4;)
                br 1 (;@5;)
              end
              i32.const 0
              local.set 3
              local.get 7
              i32.eqz
              br_if 1 (;@4;)
            end
            block  ;; label = @5
              block  ;; label = @6
                local.get 0
                local.get 6
                i32.load offset=28
                local.tee 4
                i32.const 2
                i32.shl
                i32.add
                i32.const 304
                i32.add
                local.tee 1
                i32.load
                local.get 6
                i32.ne
                if  ;; label = @7
                  local.get 7
                  i32.const 16
                  i32.add
                  local.get 7
                  i32.load offset=16
                  local.get 6
                  i32.ne
                  i32.const 2
                  i32.shl
                  i32.add
                  local.get 3
                  i32.store
                  local.get 3
                  br_if 1 (;@6;)
                  br 3 (;@4;)
                end
                local.get 1
                local.get 3
                i32.store
                local.get 3
                i32.eqz
                br_if 1 (;@5;)
              end
              local.get 3
              local.get 7
              i32.store offset=24
              local.get 6
              i32.load offset=16
              local.tee 1
              if  ;; label = @6
                local.get 3
                local.get 1
                i32.store offset=16
                local.get 1
                local.get 3
                i32.store offset=24
              end
              local.get 6
              i32.const 20
              i32.add
              i32.load
              local.tee 1
              i32.eqz
              br_if 1 (;@4;)
              local.get 3
              i32.const 20
              i32.add
              local.get 1
              i32.store
              local.get 1
              local.get 3
              i32.store offset=24
              br 1 (;@4;)
            end
            local.get 0
            local.get 0
            i32.load offset=4
            i32.const -2
            local.get 4
            i32.rotl
            i32.and
            i32.store offset=4
          end
          local.get 2
          local.get 5
          i32.add
          local.get 5
          i32.store
          local.get 2
          local.get 5
          i32.const 1
          i32.or
          i32.store offset=4
          local.get 2
          local.get 0
          i32.const 20
          i32.add
          i32.load
          i32.ne
          br_if 0 (;@3;)
          local.get 0
          local.get 5
          i32.store offset=8
          return
        end
        block (result i32)  ;; label = @3
          block  ;; label = @4
            block (result i32)  ;; label = @5
              block  ;; label = @6
                local.get 5
                i32.const 255
                i32.le_u
                if  ;; label = @7
                  local.get 0
                  local.get 5
                  i32.const 3
                  i32.shr_u
                  local.tee 4
                  i32.const 3
                  i32.shl
                  i32.add
                  i32.const 40
                  i32.add
                  local.set 1
                  local.get 0
                  i32.load
                  local.tee 5
                  i32.const 1
                  local.get 4
                  i32.shl
                  local.tee 4
                  i32.and
                  i32.eqz
                  br_if 1 (;@6;)
                  local.get 1
                  i32.load offset=8
                  br 2 (;@5;)
                end
                local.get 5
                i32.const 8
                i32.shr_u
                local.tee 4
                i32.eqz
                br_if 2 (;@4;)
                i32.const 31
                local.get 5
                i32.const 16777215
                i32.gt_u
                br_if 3 (;@3;)
                drop
                local.get 5
                i32.const 14
                local.get 4
                local.get 4
                i32.const 1048320
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 8
                i32.and
                local.tee 1
                i32.shl
                local.tee 4
                i32.const 520192
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 4
                i32.and
                local.tee 3
                local.get 1
                i32.or
                local.get 4
                local.get 3
                i32.shl
                local.tee 1
                i32.const 245760
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 2
                i32.and
                local.tee 4
                i32.or
                i32.sub
                local.get 1
                local.get 4
                i32.shl
                i32.const 15
                i32.shr_u
                i32.add
                local.tee 1
                i32.const 7
                i32.add
                i32.shr_u
                i32.const 1
                i32.and
                local.get 1
                i32.const 1
                i32.shl
                i32.or
                br 3 (;@3;)
              end
              local.get 0
              local.get 5
              local.get 4
              i32.or
              i32.store
              local.get 1
            end
            local.tee 0
            local.get 2
            i32.store offset=12
            local.get 1
            i32.const 8
            i32.add
            local.get 2
            i32.store
            local.get 2
            local.get 1
            i32.store offset=12
            local.get 2
            local.get 0
            i32.store offset=8
            return
          end
          i32.const 0
        end
        local.set 1
        local.get 2
        i64.const 0
        i64.store offset=16 align=4
        local.get 2
        i32.const 28
        i32.add
        local.get 1
        i32.store
        local.get 0
        local.get 1
        i32.const 2
        i32.shl
        i32.add
        i32.const 304
        i32.add
        local.set 4
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              local.get 0
              i32.load offset=4
              local.tee 3
              i32.const 1
              local.get 1
              i32.shl
              local.tee 6
              i32.and
              if  ;; label = @6
                local.get 5
                i32.const 0
                i32.const 25
                local.get 1
                i32.const 1
                i32.shr_u
                i32.sub
                local.get 1
                i32.const 31
                i32.eq
                select
                i32.shl
                local.set 1
                local.get 4
                i32.load
                local.set 3
                loop  ;; label = @7
                  local.get 3
                  local.tee 4
                  i32.load offset=4
                  i32.const -8
                  i32.and
                  local.get 5
                  i32.eq
                  br_if 3 (;@4;)
                  local.get 1
                  i32.const 29
                  i32.shr_u
                  local.set 3
                  local.get 1
                  i32.const 1
                  i32.shl
                  local.set 1
                  local.get 4
                  local.get 3
                  i32.const 4
                  i32.and
                  i32.add
                  i32.const 16
                  i32.add
                  local.tee 6
                  i32.load
                  local.tee 3
                  br_if 0 (;@7;)
                end
                local.get 6
                local.get 2
                i32.store
                local.get 2
                i32.const 24
                i32.add
                local.get 4
                i32.store
                br 1 (;@5;)
              end
              local.get 0
              i32.const 4
              i32.add
              local.get 3
              local.get 6
              i32.or
              i32.store
              local.get 4
              local.get 2
              i32.store
              local.get 2
              i32.const 24
              i32.add
              local.get 4
              i32.store
            end
            local.get 2
            local.get 2
            i32.store offset=12
            local.get 2
            local.get 2
            i32.store offset=8
            br 1 (;@3;)
          end
          local.get 4
          i32.load offset=8
          local.tee 1
          local.get 2
          i32.store offset=12
          local.get 4
          local.get 2
          i32.store offset=8
          local.get 2
          i32.const 24
          i32.add
          i32.const 0
          i32.store
          local.get 2
          local.get 4
          i32.store offset=12
          local.get 2
          local.get 1
          i32.store offset=8
        end
        local.get 0
        local.get 0
        i32.load offset=32
        i32.const -1
        i32.add
        local.tee 1
        i32.store offset=32
        local.get 1
        br_if 0 (;@2;)
        local.get 0
        i32.const 456
        i32.add
        local.set 1
        loop  ;; label = @3
          local.get 1
          i32.load
          local.tee 2
          i32.const 8
          i32.add
          local.set 1
          local.get 2
          br_if 0 (;@3;)
        end
        local.get 0
        i32.const 32
        i32.add
        i32.const -1
        i32.store
      end
    end)
  (func $.init (type 2)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        memory.size
        local.set 7
        i32.const 16
        block (result i32)  ;; label = @3
          i32.const 8
          i32.load
          local.tee 8
          local.set 1
          local.get 7
          i32.const 16
          i32.shl
          local.get 8
          i32.sub
          local.set 2
          local.get 7
          local.set 0
          i32.const 0
          local.set 6
          i32.const 12
          i32.load
          i32.eqz
          if  ;; label = @4
            call $.init_mparams
          end
          block  ;; label = @4
            local.get 2
            i32.const 521
            i32.lt_u
            br_if 0 (;@4;)
            i32.const 0
            local.set 6
            i32.const -520
            i32.const 16
            i32.load
            i32.sub
            local.get 2
            i32.le_u
            br_if 0 (;@4;)
            i32.const 0
            local.set 3
            local.get 1
            i32.const -8
            local.get 1
            i32.sub
            i32.const 7
            i32.and
            i32.const 0
            local.get 1
            i32.const 8
            i32.add
            i32.const 7
            i32.and
            select
            i32.add
            local.tee 0
            i32.const 8
            i32.add
            local.tee 6
            i32.const 0
            i32.const 480
            call $.memset
            local.set 4
            local.get 0
            i32.const 483
            i32.store offset=4
            local.get 0
            i32.const 444
            i32.add
            local.get 2
            i32.store
            local.get 0
            i32.const 440
            i32.add
            local.get 2
            i32.store
            local.get 0
            i32.const 460
            i32.add
            local.get 2
            i32.store
            local.get 0
            i32.const 40
            i32.add
            i32.const -1
            i32.store
            local.get 0
            i32.const 44
            i32.add
            i32.const 12
            i32.load
            i32.store
            local.get 0
            i32.const 24
            i32.add
            local.get 1
            i32.store
            local.get 0
            i32.const 456
            i32.add
            local.get 1
            i32.store
            local.get 0
            i32.const 472
            i32.add
            i32.const 0
            i32.store
            i32.const 32
            i32.load
            local.set 5
            local.get 0
            i32.const 476
            i32.add
            i32.const 0
            i32.store
            local.get 0
            i32.const 452
            i32.add
            local.get 5
            i32.const 4
            i32.or
            i32.store
            block  ;; label = @5
              loop  ;; label = @6
                local.get 3
                i32.const 256
                i32.eq
                br_if 1 (;@5;)
                local.get 0
                local.get 3
                i32.add
                local.tee 5
                i32.const 56
                i32.add
                local.get 5
                i32.const 48
                i32.add
                local.tee 9
                i32.store
                local.get 5
                i32.const 60
                i32.add
                local.get 9
                i32.store
                local.get 3
                i32.const 8
                i32.add
                local.set 3
                br 0 (;@6;)
              end
              unreachable
            end
            local.get 4
            local.get 4
            local.get 4
            i32.const -4
            i32.add
            i32.load
            i32.const -8
            i32.and
            i32.add
            i32.const -8
            i32.add
            local.tee 3
            local.get 1
            local.get 2
            i32.add
            i32.const -40
            i32.add
            local.get 3
            i32.sub
            call $.init_top
            local.get 4
            i32.const 8
            i32.store offset=460
          end
          local.get 6
        end
        i32.store
      end
    end)
  (func $.malloc (type 3) (param i32) (result i32)
    block (result i32)  ;; label = @1
      i32.const 16
      i32.load
      local.get 0
      call $.mspace_malloc
    end)
  (func $.realloc (type 1) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        i32.const 16
        i32.load
        local.set 15
        local.get 1
        local.set 9
        block  ;; label = @3
          block  ;; label = @4
            local.get 0
            local.tee 14
            if  ;; label = @5
              i32.const 0
              local.set 11
              local.get 9
              i32.const -65
              i32.gt_u
              br_if 2 (;@3;)
              block (result i32)  ;; label = @6
                i32.const 16
                local.get 9
                i32.const 11
                i32.add
                i32.const -8
                i32.and
                local.get 9
                i32.const 11
                i32.lt_u
                select
                local.set 3
                i32.const 0
                local.set 2
                block  ;; label = @7
                  local.get 14
                  i32.const -8
                  i32.add
                  local.tee 7
                  i32.load offset=4
                  local.tee 8
                  i32.const 3
                  i32.and
                  local.tee 10
                  i32.const 1
                  i32.eq
                  local.get 15
                  local.tee 6
                  i32.load offset=16
                  local.get 7
                  i32.gt_u
                  i32.or
                  br_if 0 (;@7;)
                  local.get 7
                  local.get 8
                  i32.const -8
                  i32.and
                  local.tee 5
                  i32.add
                  local.tee 4
                  local.get 7
                  i32.le_u
                  br_if 0 (;@7;)
                  local.get 4
                  i32.load offset=4
                  local.tee 12
                  i32.const 1
                  i32.and
                  i32.eqz
                  br_if 0 (;@7;)
                  block  ;; label = @8
                    block  ;; label = @9
                      block  ;; label = @10
                        block  ;; label = @11
                          block  ;; label = @12
                            block  ;; label = @13
                              block  ;; label = @14
                                block  ;; label = @15
                                  block  ;; label = @16
                                    local.get 10
                                    if  ;; label = @17
                                      local.get 5
                                      local.get 3
                                      i32.ge_u
                                      br_if 1 (;@16;)
                                      local.get 6
                                      i32.load offset=24
                                      local.get 4
                                      i32.eq
                                      br_if 2 (;@15;)
                                      local.get 6
                                      i32.load offset=20
                                      local.get 4
                                      i32.eq
                                      br_if 3 (;@14;)
                                      local.get 12
                                      i32.const 2
                                      i32.and
                                      br_if 10 (;@7;)
                                      local.get 12
                                      i32.const -8
                                      i32.and
                                      local.get 5
                                      i32.add
                                      local.tee 16
                                      local.get 3
                                      i32.lt_u
                                      br_if 10 (;@7;)
                                      local.get 16
                                      local.get 3
                                      i32.sub
                                      local.set 17
                                      local.get 12
                                      i32.const 255
                                      i32.gt_u
                                      br_if 4 (;@13;)
                                      local.get 4
                                      i32.load offset=12
                                      local.tee 2
                                      local.get 4
                                      i32.load offset=8
                                      local.tee 4
                                      i32.eq
                                      br_if 5 (;@12;)
                                      local.get 2
                                      local.get 4
                                      i32.store offset=8
                                      local.get 4
                                      local.get 2
                                      i32.store offset=12
                                      br 8 (;@9;)
                                    end
                                    local.get 3
                                    i32.const 256
                                    i32.lt_u
                                    br_if 9 (;@7;)
                                    local.get 5
                                    local.get 3
                                    i32.const 4
                                    i32.add
                                    i32.ge_u
                                    if  ;; label = @17
                                      local.get 7
                                      local.set 2
                                      local.get 5
                                      local.get 3
                                      i32.sub
                                      i32.const 20
                                      i32.load
                                      i32.const 1
                                      i32.shl
                                      i32.le_u
                                      br_if 10 (;@7;)
                                    end
                                    i32.const 0
                                    br 10 (;@6;)
                                  end
                                  local.get 5
                                  local.get 3
                                  i32.sub
                                  local.tee 2
                                  i32.const 16
                                  i32.lt_u
                                  br_if 7 (;@8;)
                                  local.get 7
                                  i32.const 4
                                  i32.add
                                  local.get 8
                                  i32.const 1
                                  i32.and
                                  local.get 3
                                  i32.or
                                  i32.const 2
                                  i32.or
                                  i32.store
                                  local.get 7
                                  local.get 3
                                  i32.add
                                  local.tee 3
                                  local.get 2
                                  i32.const 3
                                  i32.or
                                  i32.store offset=4
                                  local.get 4
                                  i32.const 4
                                  i32.add
                                  local.tee 8
                                  local.get 8
                                  i32.load
                                  i32.const 1
                                  i32.or
                                  i32.store
                                  local.get 6
                                  local.get 3
                                  local.get 2
                                  call $.dispose_chunk
                                  br 7 (;@8;)
                                end
                                local.get 6
                                i32.load offset=12
                                local.get 5
                                i32.add
                                local.tee 4
                                local.get 3
                                i32.le_u
                                br_if 7 (;@7;)
                                local.get 7
                                i32.const 4
                                i32.add
                                local.get 8
                                i32.const 1
                                i32.and
                                local.get 3
                                i32.or
                                i32.const 2
                                i32.or
                                i32.store
                                local.get 6
                                i32.const 24
                                i32.add
                                local.get 7
                                local.get 3
                                i32.add
                                local.tee 2
                                i32.store
                                local.get 6
                                i32.const 12
                                i32.add
                                local.get 4
                                local.get 3
                                i32.sub
                                local.tee 6
                                i32.store
                                local.get 2
                                local.get 6
                                i32.const 1
                                i32.or
                                i32.store offset=4
                                br 6 (;@8;)
                              end
                              local.get 6
                              i32.load offset=8
                              local.get 5
                              i32.add
                              local.tee 4
                              local.get 3
                              i32.lt_u
                              br_if 6 (;@7;)
                              block  ;; label = @14
                                local.get 4
                                local.get 3
                                i32.sub
                                local.tee 2
                                i32.const 16
                                i32.ge_u
                                if  ;; label = @15
                                  local.get 7
                                  i32.const 4
                                  i32.add
                                  local.get 8
                                  i32.const 1
                                  i32.and
                                  local.get 3
                                  i32.or
                                  i32.const 2
                                  i32.or
                                  i32.store
                                  local.get 7
                                  local.get 3
                                  i32.add
                                  local.tee 3
                                  local.get 2
                                  i32.const 1
                                  i32.or
                                  i32.store offset=4
                                  local.get 7
                                  local.get 4
                                  i32.add
                                  local.tee 8
                                  local.get 2
                                  i32.store
                                  local.get 8
                                  local.get 8
                                  i32.load offset=4
                                  i32.const -2
                                  i32.and
                                  i32.store offset=4
                                  br 1 (;@14;)
                                end
                                local.get 7
                                i32.const 4
                                i32.add
                                local.get 8
                                i32.const 1
                                i32.and
                                local.get 4
                                i32.or
                                i32.const 2
                                i32.or
                                i32.store
                                local.get 7
                                local.get 4
                                i32.add
                                local.tee 3
                                local.get 3
                                i32.load offset=4
                                i32.const 1
                                i32.or
                                i32.store offset=4
                                i32.const 0
                                local.set 2
                                i32.const 0
                                local.set 3
                              end
                              local.get 6
                              i32.const 20
                              i32.add
                              local.get 3
                              i32.store
                              local.get 6
                              i32.const 8
                              i32.add
                              local.get 2
                              i32.store
                              br 5 (;@8;)
                            end
                            local.get 4
                            i32.load offset=24
                            local.set 13
                            local.get 4
                            i32.load offset=12
                            local.tee 5
                            local.get 4
                            i32.eq
                            br_if 1 (;@11;)
                            local.get 4
                            i32.load offset=8
                            local.tee 2
                            local.get 5
                            i32.store offset=12
                            local.get 5
                            local.get 2
                            i32.store offset=8
                            local.get 13
                            br_if 2 (;@10;)
                            br 3 (;@9;)
                          end
                          local.get 6
                          local.get 6
                          i32.load
                          i32.const -2
                          local.get 12
                          i32.const 3
                          i32.shr_u
                          i32.rotl
                          i32.and
                          i32.store
                          br 2 (;@9;)
                        end
                        block  ;; label = @11
                          local.get 4
                          i32.const 20
                          i32.add
                          local.tee 2
                          i32.load
                          local.tee 10
                          i32.eqz
                          if  ;; label = @12
                            local.get 4
                            i32.const 16
                            i32.add
                            local.tee 2
                            i32.load
                            local.tee 10
                            i32.eqz
                            br_if 1 (;@11;)
                          end
                          loop  ;; label = @12
                            local.get 2
                            local.set 12
                            local.get 10
                            local.tee 5
                            i32.const 20
                            i32.add
                            local.tee 2
                            i32.load
                            local.tee 10
                            br_if 0 (;@12;)
                            local.get 5
                            i32.const 16
                            i32.add
                            local.set 2
                            local.get 5
                            i32.load offset=16
                            local.tee 10
                            br_if 0 (;@12;)
                          end
                          local.get 12
                          i32.const 0
                          i32.store
                          local.get 13
                          i32.eqz
                          br_if 2 (;@9;)
                          br 1 (;@10;)
                        end
                        i32.const 0
                        local.set 5
                        local.get 13
                        i32.eqz
                        br_if 1 (;@9;)
                      end
                      block  ;; label = @10
                        block  ;; label = @11
                          local.get 6
                          local.get 4
                          i32.load offset=28
                          local.tee 10
                          i32.const 2
                          i32.shl
                          i32.add
                          i32.const 304
                          i32.add
                          local.tee 2
                          i32.load
                          local.get 4
                          i32.ne
                          if  ;; label = @12
                            local.get 13
                            i32.const 16
                            i32.add
                            local.get 13
                            i32.load offset=16
                            local.get 4
                            i32.ne
                            i32.const 2
                            i32.shl
                            i32.add
                            local.get 5
                            i32.store
                            local.get 5
                            br_if 1 (;@11;)
                            br 3 (;@9;)
                          end
                          local.get 2
                          local.get 5
                          i32.store
                          local.get 5
                          i32.eqz
                          br_if 1 (;@10;)
                        end
                        local.get 5
                        local.get 13
                        i32.store offset=24
                        local.get 4
                        i32.load offset=16
                        local.tee 2
                        if  ;; label = @11
                          local.get 5
                          local.get 2
                          i32.store offset=16
                          local.get 2
                          local.get 5
                          i32.store offset=24
                        end
                        local.get 4
                        i32.const 20
                        i32.add
                        i32.load
                        local.tee 2
                        i32.eqz
                        br_if 1 (;@9;)
                        local.get 5
                        i32.const 20
                        i32.add
                        local.get 2
                        i32.store
                        local.get 2
                        local.get 5
                        i32.store offset=24
                        br 1 (;@9;)
                      end
                      local.get 6
                      local.get 6
                      i32.load offset=4
                      i32.const -2
                      local.get 10
                      i32.rotl
                      i32.and
                      i32.store offset=4
                    end
                    local.get 17
                    i32.const 15
                    i32.le_u
                    if  ;; label = @9
                      local.get 7
                      i32.const 4
                      i32.add
                      local.get 16
                      local.get 8
                      i32.const 1
                      i32.and
                      i32.or
                      i32.const 2
                      i32.or
                      i32.store
                      local.get 7
                      local.get 16
                      i32.add
                      local.tee 6
                      local.get 6
                      i32.load offset=4
                      i32.const 1
                      i32.or
                      i32.store offset=4
                      br 1 (;@8;)
                    end
                    local.get 7
                    i32.const 4
                    i32.add
                    local.get 8
                    i32.const 1
                    i32.and
                    local.get 3
                    i32.or
                    i32.const 2
                    i32.or
                    i32.store
                    local.get 7
                    local.get 3
                    i32.add
                    local.tee 3
                    local.get 17
                    i32.const 3
                    i32.or
                    i32.store offset=4
                    local.get 7
                    local.get 16
                    i32.add
                    local.tee 2
                    local.get 2
                    i32.load offset=4
                    i32.const 1
                    i32.or
                    i32.store offset=4
                    local.get 6
                    local.get 3
                    local.get 17
                    call $.dispose_chunk
                  end
                  local.get 7
                  local.set 2
                end
                local.get 2
              end
              local.tee 18
              i32.eqz
              br_if 1 (;@4;)
              local.get 18
              i32.const 8
              i32.add
              br 3 (;@2;)
            end
            local.get 15
            local.get 9
            call $.mspace_malloc
            br 2 (;@2;)
          end
          local.get 15
          local.get 9
          call $.mspace_malloc
          local.tee 18
          i32.eqz
          br_if 0 (;@3;)
          local.get 18
          local.get 14
          local.get 14
          i32.const -4
          i32.add
          i32.load
          local.tee 11
          i32.const -8
          i32.and
          i32.const 4
          i32.const 8
          local.get 11
          i32.const 3
          i32.and
          select
          i32.sub
          local.tee 11
          local.get 9
          local.get 11
          local.get 9
          i32.lt_u
          select
          call $.memcpy
          local.set 9
          local.get 15
          local.get 14
          call $.mspace_free
          local.get 9
          local.set 11
        end
        local.get 11
      end
    end)
  (func $.free (type 7) (param i32)
    block  ;; label = @1
      i32.const 16
      i32.load
      local.get 0
      call $.mspace_free
    end)
  (func $.init_top (type 8) (param i32 i32 i32)
    (local i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        i32.const -8
        local.get 1
        i32.sub
        i32.const 7
        i32.and
        i32.const 0
        local.get 1
        i32.const 8
        i32.add
        i32.const 7
        i32.and
        select
        local.tee 3
        i32.add
        local.tee 4
        local.get 2
        local.get 3
        i32.sub
        local.tee 3
        i32.const 1
        i32.or
        i32.store offset=4
        local.get 0
        i32.const 28
        i32.load
        i32.store offset=28
        local.get 0
        local.get 3
        i32.store offset=12
        local.get 0
        local.get 4
        i32.store offset=24
        local.get 1
        local.get 2
        i32.add
        i32.const 40
        i32.store offset=4
      end
    end)
  (func $.segment_holding (type 1) (param i32 i32) (result i32)
    (local i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        local.get 0
        i32.const 448
        i32.add
        local.set 0
        block  ;; label = @3
          loop  ;; label = @4
            local.get 0
            i32.load
            local.tee 2
            local.get 1
            i32.le_u
            if  ;; label = @5
              local.get 2
              local.get 0
              i32.load offset=4
              i32.add
              local.get 1
              i32.gt_u
              br_if 2 (;@3;)
            end
            local.get 0
            i32.load offset=8
            local.tee 0
            br_if 0 (;@4;)
          end
          i32.const 0
          local.set 0
        end
        local.get 0
      end
    end)
  (func $.morecore (type 3) (param i32) (result i32)
    (local i32)
    block (result i32)  ;; label = @1
      block (result i32)  ;; label = @2
        memory.size
        local.set 1
        block  ;; label = @3
          block  ;; label = @4
            local.get 0
            i32.const 1
            i32.ge_s
            if  ;; label = @5
              local.get 0
              i32.const -1
              i32.add
              i32.const 16
              i32.shr_s
              i32.const 1
              i32.add
              memory.grow
              br_if 1 (;@4;)
              i32.const -1
              return
            end
            local.get 0
            i32.const 0
            i32.lt_s
            br_if 1 (;@3;)
          end
          local.get 1
          i32.const 16
          i32.shl
          return
        end
        i32.const -1
      end
    end)
  (func $.dispose_chunk (type 8) (param i32 i32 i32)
    (local i32 i32 i32 i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        local.get 2
        i32.add
        local.set 6
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                block  ;; label = @7
                  block  ;; label = @8
                    block  ;; label = @9
                      block  ;; label = @10
                        block  ;; label = @11
                          block  ;; label = @12
                            local.get 1
                            i32.load offset=4
                            local.tee 3
                            i32.const 1
                            i32.and
                            br_if 0 (;@12;)
                            local.get 3
                            i32.const 3
                            i32.and
                            i32.eqz
                            br_if 1 (;@11;)
                            local.get 1
                            i32.load
                            local.tee 3
                            local.get 2
                            i32.add
                            local.set 2
                            block  ;; label = @13
                              block  ;; label = @14
                                block  ;; label = @15
                                  block  ;; label = @16
                                    local.get 0
                                    i32.load offset=20
                                    local.get 1
                                    local.get 3
                                    i32.sub
                                    local.tee 1
                                    i32.ne
                                    if  ;; label = @17
                                      local.get 3
                                      i32.const 255
                                      i32.gt_u
                                      br_if 1 (;@16;)
                                      local.get 1
                                      i32.load offset=12
                                      local.tee 5
                                      local.get 1
                                      i32.load offset=8
                                      local.tee 4
                                      i32.eq
                                      br_if 2 (;@15;)
                                      local.get 5
                                      local.get 4
                                      i32.store offset=8
                                      local.get 4
                                      local.get 5
                                      i32.store offset=12
                                      br 5 (;@12;)
                                    end
                                    local.get 6
                                    i32.load offset=4
                                    local.tee 3
                                    i32.const 3
                                    i32.and
                                    i32.const 3
                                    i32.ne
                                    br_if 4 (;@12;)
                                    local.get 6
                                    i32.const 4
                                    i32.add
                                    local.get 3
                                    i32.const -2
                                    i32.and
                                    i32.store
                                    local.get 1
                                    local.get 2
                                    i32.const 1
                                    i32.or
                                    i32.store offset=4
                                    local.get 0
                                    local.get 2
                                    i32.store offset=8
                                    local.get 6
                                    local.get 2
                                    i32.store
                                    return
                                  end
                                  local.get 1
                                  i32.load offset=24
                                  local.set 7
                                  local.get 1
                                  i32.load offset=12
                                  local.tee 4
                                  local.get 1
                                  i32.eq
                                  br_if 1 (;@14;)
                                  local.get 1
                                  i32.load offset=8
                                  local.tee 3
                                  local.get 4
                                  i32.store offset=12
                                  local.get 4
                                  local.get 3
                                  i32.store offset=8
                                  local.get 7
                                  br_if 2 (;@13;)
                                  br 3 (;@12;)
                                end
                                local.get 0
                                local.get 0
                                i32.load
                                i32.const -2
                                local.get 3
                                i32.const 3
                                i32.shr_u
                                i32.rotl
                                i32.and
                                i32.store
                                br 2 (;@12;)
                              end
                              block  ;; label = @14
                                local.get 1
                                i32.const 20
                                i32.add
                                local.tee 3
                                i32.load
                                local.tee 5
                                i32.eqz
                                if  ;; label = @15
                                  local.get 1
                                  i32.const 16
                                  i32.add
                                  local.tee 3
                                  i32.load
                                  local.tee 5
                                  i32.eqz
                                  br_if 1 (;@14;)
                                end
                                loop  ;; label = @15
                                  local.get 3
                                  local.set 8
                                  local.get 5
                                  local.tee 4
                                  i32.const 20
                                  i32.add
                                  local.tee 3
                                  i32.load
                                  local.tee 5
                                  br_if 0 (;@15;)
                                  local.get 4
                                  i32.const 16
                                  i32.add
                                  local.set 3
                                  local.get 4
                                  i32.load offset=16
                                  local.tee 5
                                  br_if 0 (;@15;)
                                end
                                local.get 8
                                i32.const 0
                                i32.store
                                local.get 7
                                i32.eqz
                                br_if 2 (;@12;)
                                br 1 (;@13;)
                              end
                              i32.const 0
                              local.set 4
                              local.get 7
                              i32.eqz
                              br_if 1 (;@12;)
                            end
                            block  ;; label = @13
                              block  ;; label = @14
                                local.get 0
                                local.get 1
                                i32.load offset=28
                                local.tee 5
                                i32.const 2
                                i32.shl
                                i32.add
                                i32.const 304
                                i32.add
                                local.tee 3
                                i32.load
                                local.get 1
                                i32.ne
                                if  ;; label = @15
                                  local.get 7
                                  i32.const 16
                                  i32.add
                                  local.get 7
                                  i32.load offset=16
                                  local.get 1
                                  i32.ne
                                  i32.const 2
                                  i32.shl
                                  i32.add
                                  local.get 4
                                  i32.store
                                  local.get 4
                                  br_if 1 (;@14;)
                                  br 3 (;@12;)
                                end
                                local.get 3
                                local.get 4
                                i32.store
                                local.get 4
                                i32.eqz
                                br_if 1 (;@13;)
                              end
                              local.get 4
                              local.get 7
                              i32.store offset=24
                              local.get 1
                              i32.load offset=16
                              local.tee 3
                              if  ;; label = @14
                                local.get 4
                                local.get 3
                                i32.store offset=16
                                local.get 3
                                local.get 4
                                i32.store offset=24
                              end
                              local.get 1
                              i32.const 20
                              i32.add
                              i32.load
                              local.tee 3
                              i32.eqz
                              br_if 1 (;@12;)
                              local.get 4
                              i32.const 20
                              i32.add
                              local.get 3
                              i32.store
                              local.get 3
                              local.get 4
                              i32.store offset=24
                              br 1 (;@12;)
                            end
                            local.get 0
                            local.get 0
                            i32.load offset=4
                            i32.const -2
                            local.get 5
                            i32.rotl
                            i32.and
                            i32.store offset=4
                          end
                          block  ;; label = @12
                            local.get 6
                            i32.load offset=4
                            local.tee 3
                            i32.const 2
                            i32.and
                            i32.eqz
                            if  ;; label = @13
                              local.get 0
                              i32.load offset=24
                              local.get 6
                              i32.eq
                              br_if 1 (;@12;)
                              local.get 0
                              i32.load offset=20
                              local.get 6
                              i32.eq
                              br_if 3 (;@10;)
                              local.get 3
                              i32.const -8
                              i32.and
                              local.get 2
                              i32.add
                              local.set 2
                              local.get 3
                              i32.const 255
                              i32.gt_u
                              br_if 4 (;@9;)
                              local.get 6
                              i32.load offset=12
                              local.tee 5
                              local.get 6
                              i32.load offset=8
                              local.tee 4
                              i32.eq
                              br_if 6 (;@7;)
                              local.get 5
                              local.get 4
                              i32.store offset=8
                              local.get 4
                              local.get 5
                              i32.store offset=12
                              br 9 (;@4;)
                            end
                            local.get 6
                            i32.const 4
                            i32.add
                            local.get 3
                            i32.const -2
                            i32.and
                            i32.store
                            local.get 1
                            local.get 2
                            i32.const 1
                            i32.or
                            i32.store offset=4
                            local.get 1
                            local.get 2
                            i32.add
                            local.get 2
                            i32.store
                            br 9 (;@3;)
                          end
                          local.get 0
                          i32.const 24
                          i32.add
                          local.get 1
                          i32.store
                          local.get 0
                          local.get 0
                          i32.load offset=12
                          local.get 2
                          i32.add
                          local.tee 2
                          i32.store offset=12
                          local.get 1
                          local.get 2
                          i32.const 1
                          i32.or
                          i32.store offset=4
                          local.get 1
                          local.get 0
                          i32.load offset=20
                          i32.eq
                          br_if 3 (;@8;)
                        end
                        return
                      end
                      local.get 1
                      local.get 0
                      i32.load offset=8
                      local.get 2
                      i32.add
                      local.tee 2
                      i32.const 1
                      i32.or
                      i32.store offset=4
                      local.get 0
                      i32.const 20
                      i32.add
                      local.get 1
                      i32.store
                      local.get 0
                      local.get 2
                      i32.store offset=8
                      local.get 1
                      local.get 2
                      i32.add
                      local.get 2
                      i32.store
                      return
                    end
                    local.get 6
                    i32.load offset=24
                    local.set 7
                    local.get 6
                    i32.load offset=12
                    local.tee 4
                    local.get 6
                    i32.eq
                    br_if 2 (;@6;)
                    local.get 6
                    i32.load offset=8
                    local.tee 3
                    local.get 4
                    i32.store offset=12
                    local.get 4
                    local.get 3
                    i32.store offset=8
                    local.get 7
                    br_if 3 (;@5;)
                    br 4 (;@4;)
                  end
                  local.get 0
                  i32.const 0
                  i32.store offset=8
                  local.get 0
                  i32.const 20
                  i32.add
                  i32.const 0
                  i32.store
                  return
                end
                local.get 0
                local.get 0
                i32.load
                i32.const -2
                local.get 3
                i32.const 3
                i32.shr_u
                i32.rotl
                i32.and
                i32.store
                br 2 (;@4;)
              end
              block  ;; label = @6
                local.get 6
                i32.const 20
                i32.add
                local.tee 3
                i32.load
                local.tee 5
                i32.eqz
                if  ;; label = @7
                  local.get 6
                  i32.const 16
                  i32.add
                  local.tee 3
                  i32.load
                  local.tee 5
                  i32.eqz
                  br_if 1 (;@6;)
                end
                loop  ;; label = @7
                  local.get 3
                  local.set 8
                  local.get 5
                  local.tee 4
                  i32.const 20
                  i32.add
                  local.tee 3
                  i32.load
                  local.tee 5
                  br_if 0 (;@7;)
                  local.get 4
                  i32.const 16
                  i32.add
                  local.set 3
                  local.get 4
                  i32.load offset=16
                  local.tee 5
                  br_if 0 (;@7;)
                end
                local.get 8
                i32.const 0
                i32.store
                local.get 7
                i32.eqz
                br_if 2 (;@4;)
                br 1 (;@5;)
              end
              i32.const 0
              local.set 4
              local.get 7
              i32.eqz
              br_if 1 (;@4;)
            end
            block  ;; label = @5
              block  ;; label = @6
                local.get 0
                local.get 6
                i32.load offset=28
                local.tee 5
                i32.const 2
                i32.shl
                i32.add
                i32.const 304
                i32.add
                local.tee 3
                i32.load
                local.get 6
                i32.ne
                if  ;; label = @7
                  local.get 7
                  i32.const 16
                  i32.add
                  local.get 7
                  i32.load offset=16
                  local.get 6
                  i32.ne
                  i32.const 2
                  i32.shl
                  i32.add
                  local.get 4
                  i32.store
                  local.get 4
                  br_if 1 (;@6;)
                  br 3 (;@4;)
                end
                local.get 3
                local.get 4
                i32.store
                local.get 4
                i32.eqz
                br_if 1 (;@5;)
              end
              local.get 4
              local.get 7
              i32.store offset=24
              local.get 6
              i32.load offset=16
              local.tee 3
              if  ;; label = @6
                local.get 4
                local.get 3
                i32.store offset=16
                local.get 3
                local.get 4
                i32.store offset=24
              end
              local.get 6
              i32.const 20
              i32.add
              i32.load
              local.tee 3
              i32.eqz
              br_if 1 (;@4;)
              local.get 4
              i32.const 20
              i32.add
              local.get 3
              i32.store
              local.get 3
              local.get 4
              i32.store offset=24
              br 1 (;@4;)
            end
            local.get 0
            local.get 0
            i32.load offset=4
            i32.const -2
            local.get 5
            i32.rotl
            i32.and
            i32.store offset=4
          end
          local.get 1
          local.get 2
          i32.const 1
          i32.or
          i32.store offset=4
          local.get 1
          local.get 2
          i32.add
          local.get 2
          i32.store
          local.get 1
          local.get 0
          i32.const 20
          i32.add
          i32.load
          i32.ne
          br_if 0 (;@3;)
          local.get 0
          local.get 2
          i32.store offset=8
          return
        end
        block (result i32)  ;; label = @3
          block  ;; label = @4
            block (result i32)  ;; label = @5
              block  ;; label = @6
                local.get 2
                i32.const 255
                i32.le_u
                if  ;; label = @7
                  local.get 0
                  local.get 2
                  i32.const 3
                  i32.shr_u
                  local.tee 3
                  i32.const 3
                  i32.shl
                  i32.add
                  i32.const 40
                  i32.add
                  local.set 2
                  local.get 0
                  i32.load
                  local.tee 5
                  i32.const 1
                  local.get 3
                  i32.shl
                  local.tee 3
                  i32.and
                  i32.eqz
                  br_if 1 (;@6;)
                  local.get 2
                  i32.load offset=8
                  br 2 (;@5;)
                end
                local.get 2
                i32.const 8
                i32.shr_u
                local.tee 5
                i32.eqz
                br_if 2 (;@4;)
                i32.const 31
                local.get 2
                i32.const 16777215
                i32.gt_u
                br_if 3 (;@3;)
                drop
                local.get 2
                i32.const 14
                local.get 5
                local.get 5
                i32.const 1048320
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 8
                i32.and
                local.tee 3
                i32.shl
                local.tee 5
                i32.const 520192
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 4
                i32.and
                local.tee 4
                local.get 3
                i32.or
                local.get 5
                local.get 4
                i32.shl
                local.tee 3
                i32.const 245760
                i32.add
                i32.const 16
                i32.shr_u
                i32.const 2
                i32.and
                local.tee 5
                i32.or
                i32.sub
                local.get 3
                local.get 5
                i32.shl
                i32.const 15
                i32.shr_u
                i32.add
                local.tee 3
                i32.const 7
                i32.add
                i32.shr_u
                i32.const 1
                i32.and
                local.get 3
                i32.const 1
                i32.shl
                i32.or
                br 3 (;@3;)
              end
              local.get 0
              local.get 5
              local.get 3
              i32.or
              i32.store
              local.get 2
            end
            local.tee 3
            local.get 1
            i32.store offset=12
            local.get 2
            i32.const 8
            i32.add
            local.get 1
            i32.store
            local.get 1
            local.get 2
            i32.store offset=12
            local.get 1
            local.get 3
            i32.store offset=8
            return
          end
          i32.const 0
        end
        local.set 3
        local.get 1
        i64.const 0
        i64.store offset=16 align=4
        local.get 1
        i32.const 28
        i32.add
        local.get 3
        i32.store
        local.get 0
        local.get 3
        i32.const 2
        i32.shl
        i32.add
        i32.const 304
        i32.add
        local.set 5
        block  ;; label = @3
          block  ;; label = @4
            local.get 0
            i32.load offset=4
            local.tee 4
            i32.const 1
            local.get 3
            i32.shl
            local.tee 6
            i32.and
            if  ;; label = @5
              local.get 2
              i32.const 0
              i32.const 25
              local.get 3
              i32.const 1
              i32.shr_u
              i32.sub
              local.get 3
              i32.const 31
              i32.eq
              select
              i32.shl
              local.set 3
              local.get 5
              i32.load
              local.set 4
              loop  ;; label = @6
                local.get 4
                local.tee 5
                i32.load offset=4
                i32.const -8
                i32.and
                local.get 2
                i32.eq
                br_if 3 (;@3;)
                local.get 3
                i32.const 29
                i32.shr_u
                local.set 4
                local.get 3
                i32.const 1
                i32.shl
                local.set 3
                local.get 5
                local.get 4
                i32.const 4
                i32.and
                i32.add
                i32.const 16
                i32.add
                local.tee 0
                i32.load
                local.tee 4
                br_if 0 (;@6;)
              end
              local.get 0
              local.get 1
              i32.store
              local.get 1
              i32.const 24
              i32.add
              local.get 5
              i32.store
              br 1 (;@4;)
            end
            local.get 0
            i32.const 4
            i32.add
            local.get 4
            local.get 6
            i32.or
            i32.store
            local.get 5
            local.get 1
            i32.store
            local.get 1
            i32.const 24
            i32.add
            local.get 5
            i32.store
          end
          local.get 1
          local.get 1
          i32.store offset=12
          local.get 1
          local.get 1
          i32.store offset=8
          return
        end
        local.get 5
        i32.load offset=8
        local.tee 2
        local.get 1
        i32.store offset=12
        local.get 5
        local.get 1
        i32.store offset=8
        local.get 1
        i32.const 24
        i32.add
        i32.const 0
        i32.store
        local.get 1
        local.get 5
        i32.store offset=12
        local.get 1
        local.get 2
        i32.store offset=8
      end
    end)
  (func $.start (type 2)
    (local i32)
    block  ;; label = @1
      call $.init
      block (result i32)  ;; label = @2
        i32.const 24
        call $.malloc
        i32.const 0
        i32.const 24
        call $.memset
        local.tee 0
        i32.const 4
        i32.store
        local.get 0
        i32.const 4
        i32.store offset=4
        local.get 0
        i32.const 32
        i32.store offset=8
        local.get 0
        i32.const 56
        i32.store offset=12
        local.get 0
        i32.const 80
        i32.store offset=16
        local.get 0
        i32.const 104
        i32.store offset=20
        local.get 0
      end
      global.set 3
      global.get 3
      i32.load offset=4
      global.set 1
      i32.const 128
      global.set 0
      block  ;; label = @2
        i32.const 0
        global.set 2
        loop  ;; label = @3
          global.get 2
          global.get 1
          i32.lt_s
          if  ;; label = @4
            block  ;; label = @5
              global.get 0
              i32.const 144
              i32.add
              global.set 0
              global.get 0
              global.get 3
              global.get 2
              i32.const 4
              i32.mul
              i32.add
              i32.load offset=8
              i32.add
              global.set 0
              global.get 0
              i32.const 160
              i32.add
              global.set 0
            end
            global.get 2
            i32.const 1
            i32.add
            global.set 2
            br 1 (;@3;)
          end
        end
      end
      global.get 0
      i32.const 184
      i32.add
      global.set 0
    end)
  (table (;0;) 0 funcref)
  (memory (;0;) 1)
  (global (;0;) (mut i32) (i32.const 0))
  (global (;1;) (mut i32) (i32.const 0))
  (global (;2;) (mut i32) (i32.const 0))
  (global (;3;) (mut i32) (i32.const 0))
  (export "memory" (memory 0))
  (export "malloc" (func $.malloc))
  (export "free" (func $.free))
  (start $.start)
  (data (;0;) (i32.const 8) "\80\00\00\00\00\00\00\00")
  (data (;1;) (i32.const 32) "\06\00\00\00\06\00\00\00B\00a\00n\00a\00n\00a\00")
  (data (;2;) (i32.const 56) "\06\00\00\00\06\00\00\00O\00r\00a\00n\00g\00e\00")
  (data (;3;) (i32.const 80) "\05\00\00\00\05\00\00\00A\00p\00p\00l\00e\00")
  (data (;4;) (i32.const 104) "\05\00\00\00\05\00\00\00M\00a\00n\00g\00o\00"))
